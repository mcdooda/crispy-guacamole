#ifndef GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H
#define GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H

#include <flat.h>
#include "projectilecomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace map
{
class Map;
}
namespace entity
{
class Entity;
class EntityUpdater;

namespace component
{
namespace projectile
{

class ProjectileComponent : public ComponentImpl<ProjectileComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "projectile"; }
		inline static const char* getVisualName() { return "Projectile"; }
		inline static bool enableInMapEditor() { return false; }
		inline static bool enableInEntityEditor() { return false; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		void setSpeed(const flat::Vector3& speed);
		inline const flat::Vector3& getSpeed() const { return m_speed; }

		inline void setTarget(Entity* target) { m_target = target; }

		inline void setWeight(float weight) { m_weight = weight; }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);
		bool removedFromMap(Entity* entity);
		bool headingChanged(float heading, float previousHeading);
		bool elevationChanged(float elevation);

		void checkTargetCollision();

		bool collided(Entity* collidedEntity, map::TileIndex collidedTileIndex, const flat::Vector3& normal);
		bool collidedWithEntity(Entity* collidedEntity, const flat::Vector3& normal);
		bool collidedWithMap(map::TileIndex tileIndex, const flat::Vector3& normal);

		float getSpeedXY() const;

		bool updateSpritePosition(const flat::Vector3& position);
		bool updateSpriteElevation(float elevation);
		void updateSpriteRotation() const;
		
	private:
		EntityHandle m_target;
		flat::Vector3 m_speed;
		float m_weight;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H


