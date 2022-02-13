#include "propcomponent.h"
#include "propcomponenttemplate.h"

#include "entity/entity.h"
#include "entity/entityupdater.h"
#include "entity/component/components/sprite/spritecomponent.h"
#include "entity/entitytemplate.h"

#include "map/map.h"
#include "map/tile.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

void PropComponent::init()
{
	m_owner->addedToMap.on(this, &PropComponent::addedToMap);
	m_owner->removedFromMap.on(this, &PropComponent::removedFromMap);
}

void PropComponent::deinit()
{
	m_owner->addedToMap.off(this);
	m_owner->removedFromMap.off(this);
}

void PropComponent::selectAnimation(
	const EntityTemplate* topLeftTemplate,
	const EntityTemplate* topRightTemplate,
	const EntityTemplate* bottomLeftTemplate,
	const EntityTemplate* bottomRightTemplate)
{
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& selectAnimation = getTemplate()->getSelectAnimation();
	if (selectAnimation)
	{
		selectAnimation.callFunction(
			[topLeftTemplate, topRightTemplate, bottomLeftTemplate, bottomRightTemplate](lua_State* L)
			{
				if (topLeftTemplate != nullptr)
				{
					lua_pushstring(L, topLeftTemplate->getPath().stem().string().c_str());
				}
				else
				{
					lua_pushnil(L);
				}

				if (topRightTemplate != nullptr)
				{
					lua_pushstring(L, topRightTemplate->getPath().stem().string().c_str());
				}
				else
				{
					lua_pushnil(L);
				}

				if (bottomLeftTemplate != nullptr)
				{
					lua_pushstring(L, bottomLeftTemplate->getPath().stem().string().c_str());
				}
				else
				{
					lua_pushnil(L);
				}

				if (bottomRightTemplate != nullptr)
				{
					lua_pushstring(L, bottomRightTemplate->getPath().stem().string().c_str());
				}
				else
				{
					lua_pushnil(L);
				}
			},
			1,
			[this](lua_State* L)
			{
				const char* animationName = luaL_checkstring(L, 1);
				sprite::SpriteComponent* spriteComponent = m_owner->getComponent<sprite::SpriteComponent>();
				spriteComponent->setCycleAnimationByName(animationName);
			}
		);
	}
}

bool PropComponent::addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater)
{
	FLAT_ASSERT(entity == m_owner);

	const map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
	FLAT_ASSERT(map::isValidTile(tileIndex));
	const flat::Vector2i& tilePosition = map->getTileXY(tileIndex);

	const PropComponentTemplate* propComponentTemplate = getTemplate();
	const int width = propComponentTemplate->getWidth();
	const int height = propComponentTemplate->getHeight();
	const map::Navigability navigability = getTemplate()->getNavigability();

	// occupy the tile unless it's a ghost entity
	if (!m_owner->isGhost())
	{
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				const flat::Vector2 positionToOccupy(tilePosition.x - i, tilePosition.y - j);
				const map::TileIndex tileToOccupyIndex = map->findTileIndexIfNavigable(positionToOccupy, map::Navigability::ALL);
				if (map::isValidTile(tileToOccupyIndex))
				{
					map->setTileNavigability(tileToOccupyIndex, navigability);
				}
			}
		}
	}

	// center the entity on its tile
	flat::Vector3 position = m_owner->getPosition();
	position.x = std::round(position.x);
	position.y = std::round(position.y);
	position.z = map->getTileZ(tileIndex);
	m_owner->setPosition(position);

	// set aabb
	flat::AABB3 aabb;
	aabb.min = position - flat::Vector3(width - 0.5f, height - 0.5f, 0.f);
	aabb.max = position + flat::Vector3(0.5f, 0.5f, 5.f);
	m_owner->setWorldSpaceAABB(aabb);
	m_owner->setAABBCanChange(false);

	//  update sprite
	if (m_owner->getSprite() != nullptr)
	{
		flat::render::BaseSprite* sprite = m_owner->getSprite();
		flat::Vector2 position2d(map->getTransform() * position);
		sprite->setPosition(position2d);
	}

	if (propComponentTemplate->getSelectAnimation())
	{
		entityUpdater->addComponentPostCall(this, &PropComponent::selectAnimationPostCall);
	}

	return true;
}

bool PropComponent::removedFromMap(Entity* entity)
{
	if (!m_owner->isGhost())
	{
		map::Map* map = m_owner->getMap();
		map::TileIndex tileIndex = m_owner->getTileIndexFromPosition();
		FLAT_ASSERT(map::isValidTile(tileIndex));
		const flat::Vector2i& tilePosition = map->getTileXY(tileIndex);

		const PropComponentTemplate* propComponentTemplate = getTemplate();
		const int width = propComponentTemplate->getWidth();
		const int height = propComponentTemplate->getHeight();
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				map::TileIndex tileToOccupyIndex = map->getTileIndex(tilePosition.x - i, tilePosition.y - j);
				//FLAT_ASSERT(tileToOccupy != nullptr);
				if (map::isValidTile(tileToOccupyIndex))
				{
					map->resetTileNavigabilityFromTemplate(tileToOccupyIndex);
				}
			}
		}
	}
	return true;
}

void PropComponent::selectAnimationPostCall(float time, float dt)
{
	const map::Map* map = m_owner->getMap();
	flat::Vector2i position2d(m_owner->getPosition2d());
	auto getPropTemplate = [&position2d, map](int dx, int dy) -> const EntityTemplate*
	{
		flat::Vector2i adjacentPropPosition2d = position2d + flat::Vector2i(dx, dy);
		const map::TileIndex adjacentTileIndex = map->getTileIndex(adjacentPropPosition2d);
		if (map::isValidTile(adjacentTileIndex))
		{
			const EntityTemplate* propEntityTemplate = nullptr;
			map->eachTileEntity(
				adjacentTileIndex,
				[&propEntityTemplate](entity::Entity* entity)
				{
					if (entity->hasComponent<prop::PropComponent>())
					{
						propEntityTemplate = entity->getEntityTemplate().get();
					}
				}
			);
			return propEntityTemplate;
		}
		else
		{
			return nullptr;
		}
	};

	selectAnimation(
		getPropTemplate(0, -1),
		getPropTemplate(-1, 0),
		getPropTemplate(1, 0),
		getPropTemplate(0, 1)
	);
}

#ifdef FLAT_DEBUG
void PropComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add3dAABB(m_owner->getWorldSpaceAABB());
}
#endif

} // prop
} // component
} // entity
} // game



