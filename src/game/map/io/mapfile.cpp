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

} // game::map::io
