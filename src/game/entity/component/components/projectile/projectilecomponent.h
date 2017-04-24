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
namespace component
{
namespace projectile
{

class ProjectileComponent : public ComponentImpl<ProjectileComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "projectile"; }
		inline static bool enableInEditor() { return false; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool addedToMap(Entity* entity, map::Map* map);
		bool headingChanged(float heading);
		bool collided(Entity* collidedEntity);
		bool collidedWithMap();

		float getSpeedXY() const;
		
	private:
		flat::Vector3 m_speed;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H


