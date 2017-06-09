#include "tile.h"
#include "map.h"
#include "prop.h"

namespace game
{
namespace map
{

const flat::render::ProgramSettings* Tile::tileProgramSettings = nullptr;

Tile::Tile() :
	m_prop(nullptr),
	m_exists(false),
	m_walkable(true)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::~Tile()
{
	FLAT_DELETE(m_prop);
}

void Tile::setExists(Map& map, bool exists)
{
	if (m_exists != exists)
	{
		DisplayManager& displayManager = map.getDisplayManager();
		if (exists)
		{
			m_exists = true;
			displayManager.addTerrainObject(this);
			if (m_prop != nullptr)
			{
				displayManager.addTerrainObject(m_prop);
			}
		}
		else
		{
			displayManager.removeTerrainObject(this);
			m_exists = false;
			if (m_prop != nullptr)
			{
				displayManager.removeTerrainObject(m_prop);
			}
		}
	}
}

const flat::render::Sprite& Tile::getSprite() const
{
	FLAT_ASSERT(m_exists);
	return m_sprite;
}

const flat::render::ProgramSettings& Tile::getProgramSettings() const
{
	return getTileProgramSettings();
}

void Tile::updateWorldSpaceAABB()
{
	m_worldSpaceAABB.max = flat::Vector3(m_x + 0.5f, m_y + 0.5f, m_z);
	m_worldSpaceAABB.min = flat::Vector3(m_x - 0.5f, m_y - 0.5f, m_z - 100.f);
}

void Tile::setCoordinates(Map& map, int x, int y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
	updateWorldSpaceAABB();
	
	flat::Vector3 position(x, y, z);

	flat::Vector2 position2d(map.getTransform() * position);
	m_sprite.setPosition(position2d);
	m_sprite.getAABB(m_spriteAABB);

	if (m_exists)
	{
		DisplayManager& displayManager = map.getDisplayManager();
		displayManager.updateTerrainObject(this);
	}

	if (m_prop)
	{
		m_prop->setSpritePosition(position2d);
		m_prop->updateWorldSpaceAABB(position);

		if (m_exists)
		{
			DisplayManager& displayManager = map.getDisplayManager();
			displayManager.updateTerrainObject(m_prop);
		}
	}

}

void Tile::setTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& tileTexture)
{
	m_sprite.setTexture(tileTexture);
	const flat::Vector2& textureSize = tileTexture->getSize();
	flat::Vector2 origin(textureSize.x / 2, textureSize.x / 4); // should depend on tile width/height instead
	m_sprite.setOrigin(origin);
	m_sprite.getAABB(m_spriteAABB);

	if (m_exists)
	{
		DisplayManager& displayManager = map.getDisplayManager();
		displayManager.updateTerrainObject(this);
	}
}

void Tile::setPropTexture(Map& map, const std::shared_ptr<const flat::video::Texture>& propTexture)
{
	DisplayManager& displayManager = map.getDisplayManager();

	Prop* prop = m_prop;
	if (!prop)
	{
		prop = new Prop();
		prop->setSpritePosition(m_sprite.getPosition());
		prop->updateWorldSpaceAABB(flat::Vector3(m_x, m_y, m_z));
	}
	prop->setSpriteTexture(propTexture);
	const flat::Vector2& textureSize = propTexture->getSize();
	flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);
	prop->setSpriteOrigin(origin);
	setWalkable(false);

	if (m_exists)
	{
		if (m_prop != nullptr)
		{
			displayManager.updateTerrainObject(prop);
		}
		else
		{
			displayManager.addTerrainObject(prop);
		}
	}

	m_prop = prop;
}

void Tile::removeProp(Map& map)
{
	setWalkable(true);

	if (m_prop != nullptr)
	{
		if (m_exists)
		{
			DisplayManager& displayManager = map.getDisplayManager();
			displayManager.removeTerrainObject(m_prop);
		}

		FLAT_DELETE(m_prop);
	}
}

void Tile::addEntity(entity::Entity* entity)
{
	FLAT_ASSERT(std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end());
	m_entities.push_back(entity);
}

void Tile::removeEntity(entity::Entity* entity)
{
	std::vector<entity::Entity*>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
	FLAT_ASSERT(it != m_entities.end());
	m_entities.erase(it);
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

void Tile::eachWalkableNeighborTiles(const Map & map, float jumpHeight, std::function<void(const Tile*)> func) const
{
	const float maxZ = m_z + jumpHeight;

	if (const Tile* tile = map.getTileIfWalkable(m_x - 1, m_y))
	{
		if (tile->getZ() <= maxZ)
			func(tile);
	}

	if (const Tile* tile = map.getTileIfWalkable(m_x + 1, m_y))
	{
		if (tile->getZ() <= maxZ)
			func(tile);
	}

	if (const Tile* tile = map.getTileIfWalkable(m_x, m_y - 1))
	{
		if (tile->getZ() <= maxZ)
			func(tile);
	}

	if (const Tile* tile = map.getTileIfWalkable(m_x, m_y + 1))
	{
		if (tile->getZ() <= maxZ)
			func(tile);
	}
}

} // map
} // game



