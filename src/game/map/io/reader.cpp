#include <sstream>
#include "reader.h"
#include "../map.h"
#include "../tile.h"
#include "../../game.h"
#include "../../mod/mod.h"

namespace game
{
namespace map
{
namespace io
{

Reader::Reader(Game* game, const mod::Mod& mod, Map& map) :
	m_game(game),
	m_mod(mod),
	m_map(map),
	m_file(mod.getMapPath().c_str(), std::ofstream::binary)
{
	FLAT_ASSERT(m_file);
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
	readHeaders();
	readTiles();
	readEntities();
}

void Reader::readHeaders()
{
	// tile textures
	uint16_t numTiles = readUint16();
	m_tileTextures.reserve(numTiles);
	for (int i = 0; i < numTiles; ++i)
	{
		std::string name = readString();
		std::string texturePath = m_mod.getTexturePath("tiles/" + name);
		m_tileTextures.push_back(m_game->video->getTexture(texturePath));
	}

	// prop textures
	uint16_t numProps = readUint16();
	m_propTextures.reserve(numProps);
	for (int i = 0; i < numProps; ++i)
	{
		std::string name = readString();
		std::string texturePath = m_mod.getTexturePath("props/" + name);
		m_propTextures.push_back(m_game->video->getTexture(texturePath));
	}
	
	// map size
	m_mapWidth = readUint16();
	m_mapHeight = readUint16();
}

void Reader::readTiles()
{
	m_map.setSize(m_mapWidth, m_mapHeight);
	// TODO read from map.gpmap
	/*
	m_map.setAxes(
		flat::geometry::Vector2(-32.f, 16.f),
		flat::geometry::Vector2( 32.f, 16.f),
		flat::geometry::Vector2(  0.f,-32.f)
	);
	*/
	m_map.createTiles();
	
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
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
				std::shared_ptr<const flat::video::FileTexture> texture = m_tileTextures[tileIndex];
				tile->setTexture(texture);
				
				bool hasProp = readBool();
				if (hasProp)
				{
					uint16_t propIndex = readUint16();
					std::shared_ptr<const flat::video::FileTexture> texture = m_propTextures[propIndex];
					tile->setPropTexture(texture);
				}
			}
		}
	}
}

void Reader::readEntities()
{
	uint16_t numModels = readUint16();
	//m_models = new entity::EntityModel*[numModels];
	for (int i = 0; i < numModels; ++i)
	{
		std::string modelName = readString();
		//m_models[i] = entity::EntityModel::getModelByName(modelName);
	}
	
	uint16_t numEntities = readUint16();
	for (int i = 0; i < numEntities; ++i)
	{
		uint16_t modelIndex = readUint16();
		(void)modelIndex;
		//entity::EntityModel* model = m_models[modelIndex];
		float x = readFloat();
		float y = readFloat();
		
		(void)x;
		(void)y;
		
		//entity::Entity* entity;
		
		/*if (model->isUnitModel())
			entity = new entity::Unit(model, engine::Vector2d(x, y));
		
		else
			entity = new entity::Building(model, engine::Vector2d(x, y));
			
		m_map->addEntity(entity);*/
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

uint16_t Reader::readUint16()
{
	uint16_t integer;
	m_file.read(reinterpret_cast<char*>(&integer), sizeof(uint16_t));
	return integer;
}

std::string Reader::readString()
{
	uint16_t size = readUint16();
	std::string str(size, ' ');
	m_file.read(&str[0], size);
	return str;
}

} // io
} // map
} // game



