#include <sstream>
#include "reader.h"
#include "../map.h"
#include "../tile.h"
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

Reader::Reader(Game& game, const mod::Mod& mod, const std::string& mapName, Map& map) :
	m_game(game),
	m_mod(mod),
	m_mapName(mapName),
	m_map(map),
	m_file(mod.getMapPath(mapName).c_str(), std::ofstream::binary)
{
	FLAT_ASSERT(m_file.is_open());
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
	readZones();
	readEntities();
}

void Reader::readConfig()
{
	flat::Vector2 xAxis;
	flat::Vector2 yAxis;
	flat::Vector2 zAxis;

	lua_State* L = m_game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		std::string configFilePath = m_mod.getMapPath(m_mapName, "map.lua");
		luaL_loadfile(L, configFilePath.c_str());
		lua_call(L, 0, 1);

		{
			m_minX = 0;
			m_minY = 0;
			lua_getfield(L, -1, "width");
			m_maxX = luaL_checkint(L, -1) + 1;
			lua_getfield(L, -2, "height");
			m_maxY = luaL_checkint(L, -1) + 1;
			lua_pop(L, 2);
		}

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
	// tile textures
	uint16_t numTiles;
	read(numTiles);
	m_tileTextures.reserve(numTiles);
	for (int i = 0; i < numTiles; ++i)
	{
		std::string name;
		read(name);
		std::string texturePath = m_mod.getTexturePath("tiles/" + name);
		m_tileTextures.push_back(m_game.video->getTexture(texturePath));
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
	// TODO read from map.gpmap
	/*
	m_map.setAxes(
		flat::Vector2(-32.f, 16.f),
		flat::Vector2( 32.f, 16.f),
		flat::Vector2(  0.f,-32.f)
	);
	*/
	m_map.createTiles();
	
	for (int x = m_minX; x <= m_maxX; ++x)
	{
		for (int y = m_minY; y <= m_maxY; ++y)
		{
			Tile* tile = m_map.getTile(x, y);
			
			// tile actually exists?
			bool exists;
			read(exists);
			if (exists)
			{
				tile->setExists(true);

				float z;
				read(z);
				tile->setCoordinates(m_map, x, y, z);
				
				uint16_t tileIndex;
				read(tileIndex);
				const std::shared_ptr<const flat::video::FileTexture>& texture = m_tileTextures[tileIndex];
				tile->setTexture(texture);
				
				bool hasProp;
				read(hasProp);
				if (hasProp)
				{
					uint16_t propIndex;
					read(propIndex);
					const std::shared_ptr<const flat::video::FileTexture>& texture = m_propTextures[propIndex];
					tile->setPropTexture(texture);
				}
			}
		}
	}
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
		entityTemplates.push_back(entityTemplate);
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
		Tile* tile = m_map.getTile(x, y);
		FLAT_ASSERT(tile != nullptr);
		flat::Vector3 position(x, y, tile->getZ());
		baseMapState.spawnEntityAtPosition(m_game, entityTemplate, position, 0.f, 0.f);
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
			uint16_t minX, minY, maxX, maxY;
			read<uint16_t>(minX);
			read<uint16_t>(minY);
			read<uint16_t>(maxX);
			read<uint16_t>(maxY);
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


