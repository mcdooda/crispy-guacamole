#include <sstream>
#include "reader.h"
#include "../map.h"
#include "../tile.h"
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

void Reader::read(lua_State* L)
{
	readConfig(L);
	readHeaders();
	readTiles();
	readEntities();
}

void Reader::readConfig(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	std::string configFilePath = m_mod.getMapPath(m_mapName, "map.lua");
	luaL_loadfile(L, configFilePath.c_str());
	lua_call(L, 0, 1);

	flat::Vector2 xAxis;
	flat::Vector2 yAxis;
	flat::Vector2 zAxis;
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

	m_map.setAxes(xAxis, yAxis, zAxis);
}

void Reader::readHeaders()
{
	// tile textures
	uint16_t numTiles = readUint16();
	m_tileTextures.reserve(numTiles);
	for (int i = 0; i < numTiles; ++i)
	{
		std::string name;
		readString(name);
		std::string texturePath = m_mod.getTexturePath("tiles/" + name);
		m_tileTextures.push_back(m_game.video->getTexture(texturePath));
	}

	// prop textures
	uint16_t numProps = readUint16();
	m_propTextures.reserve(numProps);
	for (int i = 0; i < numProps; ++i)
	{
		std::string name;
		readString(name);
		std::string texturePath = m_mod.getTexturePath("props/" + name);
		m_propTextures.push_back(m_game.video->getTexture(texturePath));
	}
	
	// map size
	m_minX = readInt16();
	m_maxX = readInt16();
	m_minY = readInt16();
	m_maxY = readInt16();
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
			bool exists = readBool();
			if (exists)
			{
				tile->setExists(true);

				float z = readFloat();
				tile->setCoordinates(m_map, x, y, z);
				
				uint16_t tileIndex = readUint16();
				const std::shared_ptr<const flat::video::FileTexture>& texture = m_tileTextures[tileIndex];
				tile->setTexture(texture);
				
				bool hasProp = readBool();
				if (hasProp)
				{
					uint16_t propIndex = readUint16();
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

	uint16_t numEntityTemplates = readUint16();
	std::vector<std::shared_ptr<const entity::EntityTemplate>> entityTemplates;
	entityTemplates.reserve(numEntityTemplates);
	for (int i = 0; i < numEntityTemplates; ++i)
	{
		std::string entityTemplateName;
		readString(entityTemplateName);
		std::shared_ptr<const entity::EntityTemplate> entityTemplate = baseMapState.getEntityTemplate(m_game, entityTemplateName);
		entityTemplates.push_back(entityTemplate);
	}
	
	uint16_t numEntities = readUint16();
	for (int i = 0; i < numEntities; ++i)
	{
		uint16_t entityTemplateIndex = readUint16();
		std::shared_ptr<const entity::EntityTemplate> entityTemplate = entityTemplates.at(entityTemplateIndex);

		float x = readFloat();
		float y = readFloat();
		Tile* tile = m_map.getTile(x, y);
		FLAT_ASSERT(tile != nullptr);
		flat::Vector3 position(x, y, tile->getZ());
		baseMapState.spawnEntityAtPosition(m_game, entityTemplate, position);
	}
}

bool Reader::readBool()
{
	bool boolean;
	m_file.read(reinterpret_cast<char*>(&boolean), sizeof(bool));
	return boolean;
}

float Reader::readFloat()
{
	float f;
	m_file.read(reinterpret_cast<char*>(&f), sizeof(float));
	return f;
}

int16_t Reader::readInt16()
{
	int16_t integer;
	m_file.read(reinterpret_cast<char*>(&integer), sizeof(int16_t));
	return integer;
}

uint16_t Reader::readUint16()
{
	uint16_t integer;
	m_file.read(reinterpret_cast<char*>(&integer), sizeof(uint16_t));
	return integer;
}

void Reader::readString(std::string& value)
{
	uint16_t size = readUint16();
	value.resize(size);
	m_file.read(&value[0], size);
}

} // io
} // map
} // game


