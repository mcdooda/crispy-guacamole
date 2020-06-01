#ifndef GAME_MAP_IO_MAPFILE_H
#define GAME_MAP_IO_MAPFILE_H

#include <unordered_map>
#include "flat.h"

#include "map/tile.h"

namespace game::map::io
{

class MapFile : public flat::file::serialize::Serializable
{
public:
	struct Tile
	{
		static constexpr std::uint16_t INVALID_PROP = 0xFFFF;

		std::int16_t x;
		std::int16_t y;
		float z;
		union
		{
			std::uint16_t tileTemplateId;
			struct
			{
				std::uint16_t tileTemplateIndex : 10;
				std::uint16_t tileTemplateVariantIndex : 6;
			};
		};
		std::uint16_t propIndex = INVALID_PROP;
	};

	struct Entity : public flat::file::serialize::Serializable
	{
		std::uint16_t entityTemplateIndex;
		flat::Vector2 position;

		void process(flat::file::serialize::Processor& processor) override;
	};

	struct Zone : public flat::file::serialize::Serializable
	{
		struct Rectangle : public flat::file::serialize::Serializable
		{
			std::int16_t minX;
			std::int16_t minY;
			std::int16_t maxX;
			std::int16_t maxY;

			void process(flat::file::serialize::Processor& processor) override;
		};

		std::string name;
		std::uint32_t color;
		std::vector<Rectangle> rectangles;

		void process(flat::file::serialize::Processor& processor) override;
	};

public:
	void process(flat::file::serialize::Processor& processor) override;

	// axes and bounds
	inline void setXAxis(const flat::Vector2& xAxis) { m_xAxis = xAxis; }
	inline void setYAxis(const flat::Vector2& yAxis) { m_yAxis = yAxis; }
	inline void setZAxis(const flat::Vector2& zAxis) { m_zAxis = zAxis; }

	inline const flat::Vector2& getXAxis() const { return m_xAxis; }
	inline const flat::Vector2& getYAxis() const { return m_yAxis; }
	inline const flat::Vector2& getZAxis() const { return m_zAxis; }

	inline void setMinX(std::int16_t minX) { m_minX = minX; }
	inline void setMaxX(std::int16_t maxX) { m_maxX = maxX; }
	inline void setMinY(std::int16_t minY) { m_minY = minY; }
	inline void setMaxY(std::int16_t maxY) { m_maxY = maxY; }

	inline std::int16_t getMinX() const { return m_minX; }
	inline std::int16_t getMaxX() const { return m_maxX; }
	inline std::int16_t getMinY() const { return m_minY; }
	inline std::int16_t getMaxY() const { return m_maxY; }

	// tiles
	void addTile(const flat::Vector2i& tilePosition, float z, const std::filesystem::path& tileTemplatePath, std::uint16_t tileTemplateVariantIndex, const std::filesystem::path* propTemplatePath);

	inline size_t getTilesCount() const { return m_tilesByPosition.size(); }

	template <class Func>
	inline void eachTile(Func func) const;

	// entities
	void addEntity(const flat::Vector2& position, const std::filesystem::path& entityTemplatePath);

	const std::vector<std::filesystem::path>& getEntityTemplates() const { return m_entityTemplates; }
	const std::vector<Entity>& getEntities() const { return m_entities; }

	// zones
	void addZone(const std::string& name, std::uint32_t color, const std::vector<Zone::Rectangle>& rectangles);

	const std::vector<Zone>& getZones() const { return m_zones; }

private:
	void processTiles(flat::file::serialize::Processor& processor);
	Tile* getTileIfExists(const flat::Vector2i& tilePosition);
	const Tile* getTileIfExists(const flat::Vector2i& tilePosition) const;
	Tile* createTile(const flat::Vector2i& tilePosition);

	void processEntities(flat::file::serialize::Processor& processor);

	void processZones(flat::file::serialize::Processor& processor);

private:
	// tiles
	flat::Vector2 m_xAxis;
	flat::Vector2 m_yAxis;
	flat::Vector2 m_zAxis;

	std::vector<std::filesystem::path> m_tileTemplates;
	std::vector<std::filesystem::path> m_propTemplates;
	std::int16_t m_minX;
	std::int16_t m_maxX;
	std::int16_t m_minY;
	std::int16_t m_maxY;
	std::unordered_map<flat::Vector2i, Tile> m_tilesByPosition;

	// entities
	std::vector<std::filesystem::path> m_entityTemplates;
	std::vector<Entity> m_entities;

	// zones
	std::vector<Zone> m_zones;
};

template <class Func>
void MapFile::eachTile(Func func) const
{
	for (std::int16_t x = m_minX; x <= m_maxX; ++x)
	{
		for (std::int16_t y = m_minY; y <= m_maxY; ++y)
		{
			flat::Vector2i tilePosition(x, y);
			const Tile* tile = getTileIfExists(tilePosition);
			if (tile != nullptr)
			{
				const std::filesystem::path* propTemplate = nullptr;
				if (tile->propIndex != Tile::INVALID_PROP)
				{
					propTemplate = &m_propTemplates[tile->propIndex];
				}
				func(tilePosition, *tile, m_tileTemplates[tile->tileTemplateIndex], propTemplate);
			}
		}
	}
}

} // game::map::io

#endif // GAME_MAP_IO_MAPFILE_H
