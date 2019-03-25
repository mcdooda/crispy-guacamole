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

} // map
} // game



