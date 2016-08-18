#ifndef GAME_MAP_ENTITYTEMPLATE_H
#define GAME_MAP_ENTITYTEMPLATE_H

#include <memory>
#include <flat.h>
#include "sprite/description.h"

namespace game
{
class Game;
namespace entity
{
namespace behavior
{
class Behavior;
}

class EntityTemplate final
{
	public:
		EntityTemplate(Game* game, lua_State* L, const std::string& path);
		~EntityTemplate();
		
		// sprite
		inline const sprite::Description& getSpriteDescription() const { return m_spriteDescription; }
		
		// physics
		inline float getRadius() const { return m_radius; }
		inline float getSpeed() const { return m_speed; }
		inline float getJumpForce() const { return m_jumpForce; }
		inline float getWeight() const { return m_weight; }
		
		inline float getJumpHeight(float t) const { return (-m_weight / 2.f) * t * t + m_jumpForce * t; }
		inline float getJumpMaxHeight() const { return m_jumpMaxHeight; }
		inline float getJumpDuration() const { return m_jumpDuration; }
		inline float getJumpDistance() const { return m_jumpDistance; }
		
		// behavior
		const behavior::Behavior* getBehavior() const { return m_behavior; }
		
	private:
		void loadSpriteConfig(Game* game, lua_State* L, const std::string& path);
		void loadPhysicsConfig(Game* game, lua_State* L, const std::string& path);
		void loadBehaviorConfig(Game* game, lua_State* L, const std::string& path);
	
	private:
		// sprite
		sprite::Description m_spriteDescription;
		
		// physics
		float m_radius;
		float m_speed;
		float m_jumpForce;
		float m_weight;
		
		float m_jumpMaxHeight;
		float m_jumpDuration;
		float m_jumpDistance;
		
		// behavior
		behavior::Behavior* m_behavior;
};

} // entity
} // game

#endif // GAME_MAP_ENTITYTEMPLATE_H



