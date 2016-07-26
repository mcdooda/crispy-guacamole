#include "entity.h"
#include "map.h"
#include "tile.h"
#include "entitytemplate.h"

namespace game
{
namespace map
{

Entity::Entity(std::shared_ptr<const EntityTemplate> entityTemplate) :
	m_heading(0.f),
	m_template(entityTemplate),
	m_map(nullptr),
	m_tile(nullptr)
{
	const EntityTemplate* entityTemplatePtr = entityTemplate.get();
	m_sprite.setTexture(entityTemplatePtr->getAtlas());
	m_sprite.setOrigin(entityTemplatePtr->getSpriteOrigin());
	m_sprite.setAtlasSize(entityTemplatePtr->getAtlasWidth(), entityTemplatePtr->getAtlasHeight());
	m_sprite.setFrameDuration(entityTemplatePtr->getAnimationFrameDuration());
}

Entity::~Entity()
{
	
}

void Entity::setPosition(const flat::geometry::Vector3& position)
{
	m_position = position;
	if (m_map)
	{
		if (m_tile)
		{
			m_tile->removeEntity(this);
		}
		
		m_tile = getTileFromPosition();
		m_tile->addEntity(this);
		updateSpritePosition();
	}
}

void Entity::setHeading(float heading)
{
	// keep heading in [0, 2pi) range
	heading = fmodf(heading, M_PI * 2.f);
	if (heading < 0.f)
		heading += M_PI * 2.f;
	
	m_heading = heading;
	if (heading >= M_PI / 4.f && heading <= 5.f * M_PI / 4.f)
	{
		m_sprite.setFlipX(false);
	}
	else
	{
		m_sprite.setFlipX(true);
	}
}

void Entity::draw(const flat::util::RenderSettings& renderSettings, const flat::geometry::Matrix4& viewMatrix) const
{
	m_sprite.draw(renderSettings, viewMatrix);
}

void Entity::onAddedToMap(Map* map)
{
	FLAT_ASSERT(map && !m_map && !m_tile);
	m_map = map;
	m_tile = getTileFromPosition();
	m_tile->addEntity(this);
	updateSpritePosition();
}

void Entity::onRemovedFromMap()
{
	FLAT_ASSERT(m_map && m_tile);
	m_map = nullptr;
	m_tile = nullptr;
}

void Entity::update(float currentTime)
{
	m_sprite.update(currentTime);
}

Tile* Entity::getTileFromPosition()
{
	FLAT_ASSERT(m_map);
	int tileX = floor(m_position.x + 0.5f);
	int tileY = floor(m_position.y + 0.5f);
	return m_map->getTileIfExists(tileX, tileY);
}

void Entity::updateSpritePosition()
{
	FLAT_ASSERT(m_map);
	computeDepth(m_position.x, m_position.y, m_template->getRadius());
	
	const flat::geometry::Vector2 xAxis = m_map->getXAxis();
	const flat::geometry::Vector2 yAxis = m_map->getYAxis();
	const flat::geometry::Vector2 zAxis = m_map->getZAxis();
	
	flat::geometry::Vector2 position2d = xAxis * m_position.x + yAxis * m_position.y + zAxis * m_position.z;
	m_sprite.setPosition(position2d);
}

} // map
} // game


