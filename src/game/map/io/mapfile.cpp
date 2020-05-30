#include "map/io/mapfile.h"
#include "map/tile.h"

namespace game::map::io
{

void MapFile::Entity::process(flat::file::serialize::Processor& processor)
{
	processor.process(entityTemplateIndex);
	processor.process(position);
}

void MapFile::Zone::process(flat::file::serialize::Processor& processor)
{
	processor.process(name);
	processor.process(color);
	processor.process<std::uint8_t>(rectangles);
}

void MapFile::Zone::Rectangle::process(flat::file::serialize::Processor& processor)
{
	processor.process(minX);
	processor.process(minY);
	processor.process(maxX);
	processor.process(maxY);
}

void MapFile::process(flat::file::serialize::Processor& processor)
{
	double version = 0.0;
	processor.process(version);
	processTiles(processor);
	processEntities(processor);
	processZones(processor);
}

void MapFile::processTiles(flat::file::serialize::Processor& processor)
{
	processor.process(m_xAxis);
	processor.process(m_yAxis);
	processor.process(m_zAxis);

	processor.process<std::uint16_t>(m_tileTemplates);
	processor.process<std::uint16_t>(m_propTemplates);
	processor.process(m_minX);
	processor.process(m_maxX);
	processor.process(m_minY);
	processor.process(m_maxY);

	for (std::int16_t x = m_minX; x <= m_maxX; ++x)
	{
		for (std::int16_t y = m_minY; y <= m_maxY; ++y)
		{
			flat::Vector2i tilePosition(x, y);

			// tile actually exists?
			bool exists;
			Tile* tile = nullptr;
			if (processor.isWriting())
			{
				tile = getTileIfExists(tilePosition);
				exists = tile != nullptr;
			}
			processor.process(exists);
			if (exists)
			{
				if (processor.isReading())
				{
					tile = createTile(tilePosition);
				}

				processor.process(tile->z);
				processor.process(tile->tileTemplateId);
				if (processor.isReading())
				{
					bool hasProp;
					processor.process(hasProp);
					if (hasProp)
					{
						processor.process(tile->propIndex);
					}
				}
				else
				{
					bool hasProp = tile->propIndex != Tile::INVALID_PROP;
					processor.process(hasProp);
					if (hasProp)
					{
						processor.process(tile->propIndex);
					}
				}
			}
		}
	}
}

MapFile::Tile* MapFile::getTileIfExists(const flat::Vector2i& tilePosition)
{
	std::unordered_map<flat::Vector2i, Tile>::iterator it = m_tilesByPosition.find(tilePosition);
	if (it != m_tilesByPosition.end())
	{
		return &it->second;
	}
	return nullptr;
}

const MapFile::Tile* MapFile::getTileIfExists(const flat::Vector2i& tilePosition) const
{
	std::unordered_map<flat::Vector2i, Tile>::const_iterator it = m_tilesByPosition.find(tilePosition);
	if (it != m_tilesByPosition.cend())
	{
		return &it->second;
	}
	return nullptr;
}

MapFile::Tile* MapFile::createTile(const flat::Vector2i& tilePosition)
{
	Tile& tile = m_tilesByPosition[tilePosition];
	tile.x = static_cast<std::int16_t>(tilePosition.x);
	tile.y = static_cast<std::int16_t>(tilePosition.y);
	return &tile;
}

void MapFile::processEntities(flat::file::serialize::Processor& processor)
{
	processor.process<std::uint16_t>(m_entityTemplates);
	processor.process<std::uint16_t>(m_entities);
}

void MapFile::processZones(flat::file::serialize::Processor& processor)
{
	processor.process<std::uint16_t>(m_zones);
}

void MapFile::addTile(const flat::Vector2i& tilePosition, float z, const std::filesystem::path& tileTemplatePath, std::uint16_t tileTemplateVariantIndex, const std::filesystem::path* propTemplatePath)
{
	Tile* tile = createTile(tilePosition);
	tile->z = z;

	// tile template
	{
		std::vector<std::filesystem::path>::iterator it = std::find(m_tileTemplates.begin(), m_tileTemplates.end(), tileTemplatePath);
		if (it == m_tileTemplates.end())
		{
			tile->tileTemplateIndex = static_cast<std::uint16_t>(m_tileTemplates.size());
			m_tileTemplates.push_back(tileTemplatePath);
		}
		else
		{
			tile->tileTemplateIndex = static_cast<std::uint16_t>(it - m_tileTemplates.begin());
		}
		tile->tileTemplateVariantIndex = tileTemplateVariantIndex;
	}

	// prop template
	if (propTemplatePath != nullptr)
	{
		std::vector<std::filesystem::path>::iterator it = std::find(m_propTemplates.begin(), m_propTemplates.end(), *propTemplatePath);
		if (it == m_propTemplates.end())
		{
			tile->propIndex = static_cast<std::uint16_t>(m_propTemplates.size());
			m_propTemplates.push_back(*propTemplatePath);
		}
		else
		{
			tile->propIndex = static_cast<std::uint16_t>(it - m_propTemplates.begin());
		}
	}
}

void MapFile::addEntity(const flat::Vector2& position, const std::filesystem::path& entityTemplatePath)
{
	std::uint16_t entityTemplateIndex = 0;
	std::vector<std::filesystem::path>::iterator it = std::find(m_entityTemplates.begin(), m_entityTemplates.end(), entityTemplatePath);
	if (it == m_entityTemplates.end())
	{
		entityTemplateIndex = static_cast<std::uint16_t>(m_entityTemplates.size());
		m_entityTemplates.push_back(entityTemplatePath);
	}
	else
	{
		entityTemplateIndex = static_cast<std::uint16_t>(it - m_entityTemplates.begin());
	}
	Entity& entity = m_entities.emplace_back();
	entity.position = position;
	entity.entityTemplateIndex = entityTemplateIndex;
}

void MapFile::addZone(const std::string& name, std::uint32_t color, const std::vector<Zone::Rectangle>& rectangles)
{
	Zone& zone = m_zones.emplace_back();
	zone.name = name;
	zone.color = color;
	zone.rectangles = rectangles;
}

} // game::map::io
