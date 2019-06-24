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
	m_propIndex(PropIndex::INVALID_PROP)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::Tile(Tile&& tile) : MapObject(std::move(tile))
{
	m_sprite = std::move(tile.m_sprite);
	m_propIndex = tile.m_propIndex;
	tile.m_propIndex = PropIndex::INVALID_PROP;
}

Tile::~Tile()
{
	
}

Tile& Tile::operator=(Tile&& tile)
{
	MapObject::operator=(std::move(tile));
	m_sprite = std::move(tile.m_sprite);
	m_propIndex = tile.m_propIndex;
	tile.m_propIndex = PropIndex::INVALID_PROP;
	return *this;
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

void Tile::updateWorldSpaceAABB(const flat::Vector3& position)
{
	m_worldSpaceAABB.max = position + flat::Vector3(0.5f, 0.5f, 0.f);
	m_worldSpaceAABB.min = position - flat::Vector3(0.5f, 0.5f, 100.f);
}

} // map
} // game
