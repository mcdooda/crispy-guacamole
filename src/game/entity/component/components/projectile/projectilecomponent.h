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
		inline static bool enableInMapEditor() { return false; }
		inline static bool enableInEntityEditor() { return false; }

		void init(lua_State* L) override;
		void deinit(lua_State* L) override;

		void update(lua_State* L, float currentTime, float elapsedTime) override;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool addedToMap(lua_State* L, Entity* entity, map::Map* map);
		bool headingChanged(float heading);
		bool collided(lua_State* L, Entity* collidedEntity);
		bool collidedWithMap(lua_State* L);

		float getSpeedXY() const;
		
	private:
		flat::Vector3 m_speed;
};

} // projectile
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROJECTILE_PROJECTILECOMPONENT_H


