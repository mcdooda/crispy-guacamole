#include <vector>
#include <flat.h>
#include "writer.h"
#include "../map.h"
#include "../tile.h"
#include "../prop.h"
#include "../zone.h"
#include "../../entity/entity.h"
#include "../../entity/entitytemplate.h"
#include "../../game.h"
#include "../../mod/mod.h"

namespace game
{
namespace map
{
namespace io
{

Writer::Writer(Game& game, const Map& map) :
	m_game(game),
	m_map(map),
	m_file(game.mod.getMapFilePath(game.mapName, "map.gpmap").c_str(), std::ofstream::binary)
{

}

Writer::~Writer()
{
}

bool Writer::canWrite() const
{
	return m_file.is_open();
}

void Writer::write(const std::vector<entity::Entity*>& entities)
{
	writeHeaders();
	writeTiles();
	writeEntities(entities);
	writeZones();
}

static const double version;
void Writer::writeHeaders()
{
	// tile textures
	FLAT_ASSERT(m_tileTextures.empty());
	write(version);
	flat::Vector2 xAxis;
	flat::Vector2 yAxis;
	flat::Vector2 zAxis;
	m_map.getAxes(xAxis, yAxis, zAxis);
	write<const flat::Vector2&>(xAxis);
	write<const flat::Vector2&>(yAxis);
	write<const flat::Vector2&>(zAxis);

	std::vector<const flat::video::Texture*> tileTexturesOrdered;
	m_map.eachTile([this, &tileTexturesOrdered](TileIndex tileIndex)
	{
		const flat::render::BaseSprite& sprite = m_map.getTileSprite(tileIndex);
		const flat::video::Texture* tileTexture = sprite.getTexture().get();
		FLAT_ASSERT(tileTexture != nullptr);
		std::map<const flat::video::Texture*, uint16_t>::iterator it = m_tileTextures.find(tileTexture);
		if (it == m_tileTextures.end())
		{
			uint16_t index = static_cast<uint16_t>(m_tileTextures.size());
			m_tileTextures[tileTexture] = index;
			tileTexturesOrdered.push_back(tileTexture);
		}
	});

	uint16_t numTiles = static_cast<uint16_t>(tileTexturesOrdered.size());
	write(numTiles);
	for (const flat::video::Texture* tileTexture : tileTexturesOrdered)
	{
		const flat::video::FileTexture* tileFileTexture = static_cast<const flat::video::FileTexture*>(tileTexture);
		std::string tileTemplateName = getTileTemplateNameFromTexturePath(tileFileTexture->getFileName());
		write<const std::string&>(tileTemplateName);
	}

	// prop textures
	FLAT_ASSERT(m_propTextures.empty());
	std::vector<const flat::video::Texture*> propTexturesOrdered;
	m_map.eachTile([this, &propTexturesOrdered](TileIndex tileIndex)
	{
		if (const Prop* prop = m_map.getTileProp(tileIndex))
		{
			const flat::render::BaseSprite& sprite = prop->getSprite();
			const flat::video::Texture* propTexture = sprite.getTexture().get();
			FLAT_ASSERT(propTexture != nullptr);
			std::map<const flat::video::Texture*, uint16_t>::iterator it = m_propTextures.find(propTexture);
			if (it == m_propTextures.end())
			{
				uint16_t index = static_cast<uint16_t>(m_propTextures.size());
				m_propTextures[propTexture] = index;
				propTexturesOrdered.push_back(propTexture);
			}
		}
	});

	uint16_t numProps = static_cast<uint16_t>(m_propTextures.size());
	write(numProps);
	const size_t propsPrefixSize = std::string("props/").size();
	for (const flat::video::Texture* propTexture : propTexturesOrdered)
	{
		const flat::video::FileTexture* propFileTexture = static_cast<const flat::video::FileTexture*>(propTexture);
		std::filesystem::path propTextureName = m_game.mod.getTextureRelativePath(propFileTexture->getFileName()).parent_path().stem();
		write<const std::string&>(propTextureName.string());
	}

	int minX, maxX, minY, maxY;
	m_map.getActualBounds(minX, maxX, minY, maxY);
	write<int16_t>(minX);
	write<int16_t>(maxX);
	write<int16_t>(minY);
	write<int16_t>(maxY);
}

void Writer::writeTiles()
{
	int minX, maxX, minY, maxY;
	m_map.getActualBounds(minX, maxX, minY, maxY);
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			TileIndex tileIndex = m_map.getTileIndex(x, y);

			write(tileIndex != TileIndex::INVALID_TILE);
			if (tileIndex != TileIndex::INVALID_TILE)
			{
				write(m_map.getTileZ(tileIndex));

				const flat::render::SynchronizedSprite& tileSprite = static_cast<const flat::render::SynchronizedSprite&>(m_map.getTileSprite(tileIndex));
				const flat::render::SpriteSynchronizer& tileSpriteSynchronizer = tileSprite.getSynchronizer();

				const flat::video::Texture* tileTexture = tileSprite.getTexture().get();
				uint16_t tileTemplateIndex = m_tileTextures.at(tileTexture);
				FLAT_ASSERT((tileTemplateIndex & 0x0FFF) == tileTemplateIndex);
				uint16_t tileTemplateVariantIndex = tileSpriteSynchronizer.getCurrentLine();
				FLAT_ASSERT((tileTemplateVariantIndex & 0x003F) == tileTemplateVariantIndex);
				write<uint16_t>((tileTemplateIndex & 0x03FF) | (tileTemplateVariantIndex << 10));

				const Prop* prop = m_map.getTileProp(tileIndex);
				bool hasProp = prop != nullptr;
				write(hasProp);
				if (hasProp)
				{
					const flat::video::Texture* propTexture = prop->getSprite().getTexture().get();
					uint16_t propIndex = m_propTextures.at(propTexture);
					write(propIndex);
				}
			}
		}
	}
}

void Writer::writeEntities(const std::vector<entity::Entity*>& entities)
{
	std::map<const entity::EntityTemplate*, uint16_t> entityTemplates;
	std::vector<const entity::EntityTemplate*> entityTemplatesOrdered;
	for (const entity::Entity* entity : entities)
	{
		const entity::EntityTemplate* entityTemplate = entity->getEntityTemplate().get();
		FLAT_ASSERT(entityTemplate != nullptr);
		std::map<const entity::EntityTemplate*, uint16_t>::iterator it = entityTemplates.find(entityTemplate);
		if (it == entityTemplates.end())
		{
			uint16_t index = static_cast<uint16_t>(entityTemplates.size());
			entityTemplates[entityTemplate] = index;
			entityTemplatesOrdered.push_back(entityTemplate);
		}
	}

	write(static_cast<uint16_t>(entityTemplates.size()));
	for (const entity::EntityTemplate* entityTemplate : entityTemplatesOrdered)
	{
		write<const std::string&>(entityTemplate->getName());
	}

	write(static_cast<uint16_t>(entities.size()));
	for (const entity::Entity* entity : entities)
	{
		const entity::EntityTemplate* entityTemplate = entity->getEntityTemplate().get();
		FLAT_ASSERT(entityTemplate != nullptr);
		uint16_t entityTemplateIndex = entityTemplates.at(entityTemplate);
		write(entityTemplateIndex);
		const flat::Vector3& position = entity->getPosition();
		write(position.x);
		write(position.y);
	}
}

void Writer::writeZones()
{
	const std::map<std::string, std::shared_ptr<Zone>>& zones = m_map.getZones();

	write<uint16_t>(static_cast<uint16_t>(zones.size()));
	for (const std::pair<std::string, std::shared_ptr<Zone>>& pair : zones)
	{
		write<const std::string&>(pair.first);
		const Zone* zone = pair.second.get();
		write<uint32_t>(zone->getColor());
		const std::vector<Zone::Rectangle>& rectangles = zone->getRectangles();
		write<uint8_t>(static_cast<uint8_t>(rectangles.size()));
		for (const Zone::Rectangle& rectangle : rectangles)
		{
			write<uint16_t>(rectangle.minX);
			write<uint16_t>(rectangle.minY);
			write<uint16_t>(rectangle.maxX);
			write<uint16_t>(rectangle.maxY);
		}
	}
}

std::string Writer::getTileTemplateNameFromTexturePath(const std::string& texturePath) const
{
	std::filesystem::path path(texturePath);
	return path.parent_path().stem().string();
}

} // io
} // map
} // game


