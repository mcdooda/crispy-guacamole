#include <sstream>
#include "reader.h"
#include "../map.h"
#include "../tile.h"
#include "../tiletemplate.h"
#include "../zone.h"
#include "../../game.h"
#include "../../mod/mod.h"
#include "../../entity/entitytemplate.h"
#include "../../states/basemapstate.h"

namespace game
{
namespace map
{
namespace io
{

Reader::Reader(Game& game, const mod::Mod& mod, Map& map) :
	m_game(game),
	m_mod(mod),
	m_map(map),
	m_file(mod.getMapPath(game.mapName).c_str(), std::ofstream::binary)
{
	
}

Reader::~Reader()
{
	
}

bool Reader::canRead() const
{
	return m_file.is_open();
}

void Reader::read()
{
	readConfig();

	readHeaders();
	readTiles();
	readEntities();
	readZones();
}

void Reader::readConfig()
{
	flat::Vector2 xAxis;
	flat::Vector2 yAxis;
	flat::Vector2 zAxis;

	lua_State* L = m_game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		std::string configFilePath = m_mod.getMapPath(m_game.mapName, "map.lua");
		luaL_loadfile(L, configFilePath.c_str());
		lua_call(L, 0, 1);

		{
			lua_getfield(L, -1, "axes");
			luaL_checktype(L, -1, LUA_TTABLE);
			{
				lua_getfield(L, -1, "x");
				luaL_checktype(L, -1, LUA_TTABLE);
				lua_rawgeti(L, -1, 1);
				xAxis.x = static_cast<float>(luaL_checknumber(L, -1));
				lua_rawgeti(L, -2, 2);
				xAxis.y = static_cast<float>(luaL_checknumber(L, -1));
				lua_pop(L, 3);
			}
			{
				lua_getfield(L, -1, "y");
				luaL_checktype(L, -1, LUA_TTABLE);
				lua_rawgeti(L, -1, 1);
				yAxis.x = static_cast<float>(luaL_checknumber(L, -1));
				lua_rawgeti(L, -2, 2);
				yAxis.y = static_cast<float>(luaL_checknumber(L, -1));
				lua_pop(L, 3);
			}
			{
				lua_getfield(L, -1, "z");
				luaL_checktype(L, -1, LUA_TTABLE);
				lua_rawgeti(L, -1, 1);
				zAxis.x = static_cast<float>(luaL_checknumber(L, -1));
				lua_rawgeti(L, -2, 2);
				zAxis.y = static_cast<float>(luaL_checknumber(L, -1));
				lua_pop(L, 3);
			}
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	m_map.setAxes(xAxis, yAxis, zAxis);
}

void Reader::readHeaders()
{
	states::BaseMapState& baseMapState = m_game.getStateMachine().getState()->to<states::BaseMapState>();

	// tile textures
	uint16_t numTiles;
	read(numTiles);
	m_tileTemplates.reserve(numTiles);
	for (int i = 0; i < numTiles; ++i)
	{
		std::string tileTemplateName;
		read(tileTemplateName);
		std::shared_ptr<const TileTemplate> tileTemplate = baseMapState.getTileTemplate(m_game, tileTemplateName);
		m_tileTemplates.push_back(tileTemplate);
	}

	// prop textures
	uint16_t numProps;
	read(numProps);
	m_propTextures.reserve(numProps);
	for (int i = 0; i < numProps; ++i)
	{
		std::string name;
		read(name);
		std::string texturePath = m_mod.getTexturePath("props/" + name);
		m_propTextures.push_back(m_game.video->getTexture(texturePath));
	}
	
	// map size
	int16_t minX, maxX, minY, maxY;
	read(minX);
	read(maxX);
	read(minY);
	read(maxY);
	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;
}

void Reader::readTiles()
{
	m_map.setBounds(m_minX, m_maxX, m_minY, m_maxY);
	
	for (int x = m_minX; x <= m_maxX; ++x)
	{
		for (int y = m_minY; y <= m_maxY; ++y)
		{
			// tile actually exists?
			bool exists;
			read(exists);
			if (exists)
			{
				float z;
				read(z);
				
				uint16_t tileTemplateId;
				read(tileTemplateId);

				uint16_t tileTemplateIndex = tileTemplateId & 0x0FFF;
				uint16_t tileTemplateVariantIndex = tileTemplateId >> 12;

				std::shared_ptr<const TileTemplate> tileTemplate = m_tileTemplates[tileTemplateIndex];

				flat::Vector2i xy(x, y);
				TileIndex tileIndex = m_map.createTile(xy, z, tileTemplateVariantIndex, tileTemplate);
				
				bool hasProp;
				read(hasProp);
				if (hasProp)
				{
					uint16_t propIndex;
					read(propIndex);
					const std::shared_ptr<const flat::video::FileTexture>& texture = m_propTextures[propIndex];
					m_map.setTilePropTexture(tileIndex, texture);
				}
			}
		}
	}

	m_map.updateAllTilesNormals();
	m_map.buildNeighborTiles();
}

void Reader::readEntities()
{
	states::BaseMapState& baseMapState = m_game.getStateMachine().getState()->to<states::BaseMapState>();

	uint16_t numEntityTemplates;
	read(numEntityTemplates);
	std::vector<std::shared_ptr<const entity::EntityTemplate>> entityTemplates;
	entityTemplates.reserve(numEntityTemplates);
	for (int i = 0; i < numEntityTemplates; ++i)
	{
		std::string entityTemplateName;
		read(entityTemplateName);
		std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(m_game, entityTemplateName);
		if (entityTemplate->getComponentFlags() != 0)
		{
			entityTemplates.push_back(entityTemplate);
		}
		else
		{
			entityTemplates.push_back(nullptr);
			std::cerr << "Entity '" << entityTemplateName << "' does not exist anymore" << std::endl;
		}
	}
	
	uint16_t numEntities;
	read(numEntities);
	for (int i = 0; i < numEntities; ++i)
	{
		uint16_t entityTemplateIndex;
		read(entityTemplateIndex);
		std::shared_ptr<const entity::EntityTemplate> entityTemplate = entityTemplates.at(entityTemplateIndex);

		float x, y;
		read(x);
		read(y);

		if (entityTemplate != nullptr)
		{
			TileIndex tileIndex = m_map.getTileIndex(x, y);
			FLAT_ASSERT(tileIndex != TileIndex::INVALID_TILE);
			flat::Vector3 position(x, y, m_map.getTileZ(tileIndex));
			baseMapState.spawnEntityAtPosition(m_game, entityTemplate, position);
		}
	}
}

void Reader::readZones()
{
	uint16_t numZones;
	read<uint16_t>(numZones);
	for (int i = 0; i < numZones; ++i)
	{
		std::string zoneName;
		read<std::string>(zoneName);
		std::shared_ptr<Zone>& zone = m_map.addZone(zoneName);
		uint32_t color;
		read<uint32_t>(color);
		zone->setColor(flat::video::Color(color));
		uint8_t numRectangles;
		read<uint8_t>(numRectangles);
		for (uint8_t j = 0; j < numRectangles; ++j)
		{
			int16_t minX, minY, maxX, maxY;
			read<int16_t>(minX);
			read<int16_t>(minY);
			read<int16_t>(maxX);
			read<int16_t>(maxY);
			Zone::Rectangle rectangle;
			rectangle.minX = minX;
			rectangle.minY = minY;
			rectangle.maxX = maxX;
			rectangle.maxY = maxY;
			zone->addRectangle(rectangle);
		}
	}
}

} // io
} // map
} // game


