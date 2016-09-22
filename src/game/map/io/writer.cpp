#include <vector>
#include <flat.h>
#include "writer.h"
#include "../map.h"
#include "../tile.h"
#include "../prop.h"
#include "../../game.h"
#include "../../mod/mod.h"

namespace game
{
namespace map
{
namespace io
{

Writer::Writer(Game* game, const mod::Mod& mod, const std::string& mapName, const Map& map) :
	m_game(game),
	m_mod(mod),
	m_map(map),
	m_file(mod.getMapPath(mapName).c_str(), std::ofstream::binary)
{

}

Writer::~Writer()
{
}

bool Writer::canWrite() const
{
	return m_file.is_open();
}

void Writer::write()
{
	writeHeaders();
	writeTiles();
	writeEntities();
}

void Writer::writeHeaders()
{
	// tile textures
	FLAT_ASSERT(m_tileTextures.empty());
	std::vector<const flat::video::Texture*> tileTexturesOrdered;
	m_map.eachTile([this, &tileTexturesOrdered](const Tile* tile)
	{
		if (tile->exists())
		{
			const flat::video::Texture* tileTexture = tile->getSprite().getTexture().get();
			FLAT_ASSERT(tileTexture != nullptr);
			std::map<const flat::video::Texture*, uint16_t>::iterator it = m_tileTextures.find(tileTexture);
			if (it == m_tileTextures.end())
			{
				uint16_t index = static_cast<uint16_t>(m_tileTextures.size());
				m_tileTextures[tileTexture] = index;
				tileTexturesOrdered.push_back(tileTexture);
			}
		}
	});

	uint16_t numTiles = static_cast<uint16_t>(tileTexturesOrdered.size());
	writeUint16(numTiles);
	const size_t tilesPrefixSize = std::string("tiles/").size();
	for (const flat::video::Texture* tileTexture : tileTexturesOrdered)
	{
		const flat::video::FileTexture* tileFileTexture = static_cast<const flat::video::FileTexture*>(tileTexture);
		std::string tileTextureName = m_mod.getTextureRelativePath(tileFileTexture->getFileName());
		tileTextureName = tileTextureName.substr(tilesPrefixSize, tileTextureName.size() - tilesPrefixSize);
		writeString(tileTextureName);
	}

	// prop textures
	FLAT_ASSERT(m_propTextures.empty());
	std::vector<const flat::video::Texture*> propTexturesOrdered;
	m_map.eachTile([this, &propTexturesOrdered](const Tile* tile)
	{
		if (tile->exists())
		{
			if (const Prop* prop = tile->getProp())
			{
				const flat::video::Texture* propTexture = prop->getSprite().getTexture().get();
				FLAT_ASSERT(propTexture != nullptr);
				std::map<const flat::video::Texture*, uint16_t>::iterator it = m_propTextures.find(propTexture);
				if (it == m_propTextures.end())
				{
					uint16_t index = static_cast<uint16_t>(m_propTextures.size());
					m_propTextures[propTexture] = index;
					propTexturesOrdered.push_back(propTexture);
				}
			}
		}
	});

	uint16_t numProps = static_cast<uint16_t>(m_propTextures.size());
	writeUint16(numProps);
	const size_t propsPrefixSize = std::string("props/").size();
	for (const flat::video::Texture* propTexture : propTexturesOrdered)
	{
		const flat::video::FileTexture* propFileTexture = static_cast<const flat::video::FileTexture*>(propTexture);
		std::string propTextureName = m_mod.getTextureRelativePath(propFileTexture->getFileName());
		propTextureName = propTextureName.substr(propsPrefixSize, propTextureName.size() - propsPrefixSize);
		writeString(propTextureName);
	}

	writeUint16(m_map.getWidth());
	writeUint16(m_map.getHeight());
}

void Writer::writeTiles()
{
	const int mapWidth = m_map.getWidth();
	const int mapHeight = m_map.getHeight();
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			const Tile* tile = m_map.getTile(x, y);

			// tile actually exists?
			bool exists = tile->exists();
			writeBool(exists);
			if (exists)
			{
				writeFloat(tile->getZ());

				const flat::video::Texture* tileTexture = tile->getSprite().getTexture().get();
				uint16_t tileIndex = m_tileTextures.at(tileTexture);
				writeUint16(tileIndex);

				const Prop* prop = tile->getProp();
				bool hasProp = prop != nullptr;
				writeBool(hasProp);
				if (hasProp)
				{
					const flat::video::Texture* propTexture = prop->getSprite().getTexture().get();
					uint16_t propIndex = m_propTextures.at(propTexture);
					writeUint16(propIndex);
				}
			}
		}
	}
}

void Writer::writeEntities()
{
	writeUint16(0); // 0 entity template
	writeUint16(0); // 0 entity
}

void Writer::writeBool(bool value)
{
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(bool));
}

void Writer::writeFloat(float value)
{
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(float));
}

void Writer::writeUint16(uint16_t value)
{
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(uint16_t));
}

void Writer::writeString(const std::string& value)
{
	uint16_t size = static_cast<uint16_t>(value.size());
	writeUint16(size);
	m_file.write(value.data(), size);
}

} // io
} // map
} // game

