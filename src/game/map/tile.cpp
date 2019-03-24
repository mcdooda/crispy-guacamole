#include "tile.h"
#include "map.h"
#include "prop.h"
#include "displaymanager.h"

namespace game
{
namespace map
{

const flat::render::ProgramSettings* Tile::tileProgramSettings = nullptr;

Tile::Tile() :
	m_prop(nullptr),
	m_navigability(Navigability::GROUND)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::Tile(Tile&& tile)
{
	m_sprite = std::move(tile.m_sprite);
	m_prop = tile.m_prop;
	tile.m_prop = nullptr;
	m_xy = tile.m_xy;
	m_z = tile.m_z;
	m_navigability = tile.m_navigability;
}

Tile::~Tile()
{
	FLAT_DELETE(m_prop);
}

void Tile::synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer)
{
	m_sprite.synchronizeTo(spriteSynchronizer);
	flat::Vector2 origin = flat::Vector2(-map.getXAxis().x, 0.f) + (map.getXAxis() + map.getYAxis()) / 2.f;
	m_sprite.setOrigin(origin);
	m_sprite.getAABB(m_spriteAABB);

	updateRenderHash();
}

flat::render::BaseSprite& Tile::getSprite()
{
	FLAT_ASSERT(m_sprite.isSynchronized());
	return m_sprite;
}

const flat::render::ProgramSettings& Tile::getProgramSettings() const
{
	return getTileProgramSettings();
}

void Tile::updateWorldSpaceAABB()
{
	m_worldSpaceAABB.max = flat::Vector3(m_xy.x + 0.5f, m_xy.y + 0.5f, m_z);
	m_worldSpaceAABB.min = flat::Vector3(m_xy.x - 0.5f, m_xy.y - 0.5f, m_z - 100.f);
}

void Tile::setCoordinates(Map& map, const flat::Vector2i& xy, float z, bool init)
{
	m_xy = xy;
	m_z = z;
	updateWorldSpaceAABB();
	
	flat::Vector3 position(m_xy.x, m_xy.y, z);

	flat::Vector2 position2d(map.getTransform() * position);
	m_sprite.setPosition(position2d);

	m_sprite.getAABB(m_spriteAABB);

	DisplayManager& displayManager = map.getDisplayManager();
	if (init)
	{
		displayManager.addTerrainObject(this);
	}
	else
	{
		displayManager.updateTerrainObject(this);
	}
	setNearbyTilesDirty(map);

	if (m_prop)
	{
		m_prop->setSpritePosition(position2d);
		m_prop->updateWorldSpaceAABB(position);

		DisplayManager& displayManager = map.getDisplayManager();
		if (init)
		{
			displayManager.addTerrainObject(m_prop);
		}
		else
		{
			displayManager.updateTerrainObject(m_prop);
		}
	}

}

void Tile::setTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& tileTexture)
{
	FLAT_ASSERT(false);
}

void Tile::setPropTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& propTexture)
{
	DisplayManager& displayManager = map.getDisplayManager();

	Prop* prop = m_prop;
	if (!prop)
	{
		prop = new Prop();
		prop->setSpritePosition(m_sprite.getPosition());
		prop->updateWorldSpaceAABB(flat::Vector3(m_xy.x, m_xy.y, m_z));
	}
	prop->setSpriteTexture(propTexture);
	const flat::Vector2& textureSize = propTexture->getSize();
	flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);
	prop->setSpriteOrigin(origin);
	setNavigability(Navigability::NONE);

	prop->updateRenderHash();

	if (m_prop != nullptr)
	{
		displayManager.updateTerrainObject(prop);
	}
	else
	{
		displayManager.addTerrainObject(prop);
	}

	m_prop = prop;
}

void Tile::removeProp(Map& map)
{
	setNavigability(Navigability::ALL); // TODO: grab navigability from the tile template

	if (m_prop != nullptr)
	{
		DisplayManager& displayManager = map.getDisplayManager();
		displayManager.removeTerrainObject(m_prop);

		FLAT_DELETE(m_prop);
	}
}

void Tile::setColor(const flat::video::Color& color)
{
	m_sprite.setColor(color);
	if (m_prop)
	{
		m_prop->setSpriteColor(color);
	}
}

const flat::video::Color& Tile::getColor() const
{
	return m_sprite.getColor();
}

void Tile::eachNeighborTilesWithNavigability(const Map& map, float jumpHeight, Navigability navigabilityMask, std::function<void(TileIndex)> func) const
{
	const float maxZ = m_z + jumpHeight;

	auto checkTile = [&](int x, int y)
	{
		TileIndex tileIndex = map.getTileIndexIfNavigable(x, y, navigabilityMask);
		if (tileIndex != TileIndex::INVALID)
		{
			if (map.getTileZ(tileIndex) <= maxZ)
				func(tileIndex);
		}
	};

	checkTile(m_xy.x - 1, m_xy.y);
	checkTile(m_xy.x + 1, m_xy.y);
	checkTile(m_xy.x, m_xy.y - 1);
	checkTile(m_xy.x, m_xy.y + 1);
}

void Tile::setNearbyTilesDirty(Map& map)
{
	map.setTileNormalDirty(*this);

	Tile* topLeftTile = map.getTile(m_xy.x, m_xy.y - 1);
	if (topLeftTile != nullptr)
	{
		map.setTileNormalDirty(*topLeftTile);
	}

	Tile* topRightTile = map.getTile(m_xy.x - 1, m_xy.y);
	if (topRightTile != nullptr)
	{
		map.setTileNormalDirty(*topRightTile);
	}

	Tile* bottomLeftTile = map.getTile(m_xy.x + 1, m_xy.y);
	if (bottomLeftTile != nullptr)
	{
		map.setTileNormalDirty(*bottomLeftTile);
	}

	Tile* bottomRightTile = map.getTile(m_xy.x, m_xy.y + 1);
	if (bottomRightTile != nullptr)
	{
		map.setTileNormalDirty(*bottomRightTile);
	}
}

void Tile::updateNormal(Map& map)
{
	m_normalDirty = false;

	flat::Vector3 dx(1.f, 0.f, 0.f);
	flat::Vector3 dy(0.f, 1.f, 0.f);

	constexpr float minZDifference = 0.05f;

	// compute dx
	{
		Tile* bottomLeftTile = map.getTile(m_xy.x + 1, m_xy.y);
		Tile* topRightTile = map.getTile(m_xy.x - 1, m_xy.y);
		if (bottomLeftTile != nullptr && topRightTile != nullptr)
		{
			if (std::abs(m_z - bottomLeftTile->m_z) > minZDifference
				&& std::abs(m_z - topRightTile->m_z) > minZDifference)
			{
				dx.x = 2.f;
				dx.y = 0.f;
				dx.z = bottomLeftTile->m_z - topRightTile->m_z;
			}
		}
		else if (bottomLeftTile != nullptr)
		{
			if (std::abs(m_z - bottomLeftTile->m_z) > minZDifference)
			{
				dx.x = 1.f;
				dx.y = 0.f;
				dx.z = bottomLeftTile->m_z - m_z;
			}
		}
		else if (topRightTile != nullptr)
		{
			if (std::abs(m_z - topRightTile->m_z) > minZDifference)
			{
				dx.x = 1.f;
				dx.y = 0.f;
				dx.z = m_z - topRightTile->m_z;
			}
		}
	}

	// compute dy
	{
		Tile* bottomRightTile = map.getTile(m_xy.x, m_xy.y + 1);
		Tile* topLeftTile = map.getTile(m_xy.x, m_xy.y - 1);
		if (bottomRightTile != nullptr && topLeftTile != nullptr)
		{
			if (std::abs(m_z - bottomRightTile->m_z) > minZDifference
			 && std::abs(m_z - topLeftTile->m_z) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 2.f;
				dy.z = bottomRightTile->m_z - topLeftTile->m_z;
			}
		}
		else if (bottomRightTile != nullptr)
		{
			if (std::abs(m_z - bottomRightTile->m_z) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 1.f;
				dy.z = bottomRightTile->m_z - m_z;
			}
		}
		else if (topLeftTile != nullptr)
		{
			if (std::abs(m_z - topLeftTile->m_z) > minZDifference)
			{
				dy.x = 0.f;
				dy.y = 1.f;
				dy.z = m_z - topLeftTile->m_z;
			}
		}
	}

	flat::Vector3 normal = flat::normalize(flat::cross(flat::normalize(dx), flat::normalize(dy)));
	m_sprite.setNormal(normal);
}

} // map
} // game



