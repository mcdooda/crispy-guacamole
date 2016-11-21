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

		void init() override;

		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override { return true; }

	private:
		void headingChanged(float heading);
		void collided(Entity* collidedEntity);

		inline float getSpeedXY() const { return std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y); }
		
	private:
		flat::Vector3 m_speed;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H

