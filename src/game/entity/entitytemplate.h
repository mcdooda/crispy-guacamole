#ifndef GAME_MAP_ENTITYTEMPLATE_H
#define GAME_MAP_ENTITYTEMPLATE_H

#include <memory>
#include <flat.h>

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
		inline const flat::geometry::Vector2& getSpriteOrigin() const { return m_spriteOrigin; }
		inline std::shared_ptr<const flat::video::Texture> getAtlas() const { return m_atlas; }
		inline int getAtlasWidth() const { return m_atlasWidth; }
		inline int getAtlasHeight() const { return m_atlasHeight; }
		inline float getAnimationFrameDuration() const { return m_animationFrameDuration; }
		
		// physics
		inline float getRadius() const { return m_radius; }
		
		// behavior
		const behavior::Behavior* getBehavior() const { return m_behavior; }
		
	private:
		void loadSpriteConfig(Game* game, lua_State* L, const std::string& path);
		void loadPhysicsConfig(Game* game, lua_State* L, const std::string& path);
		void loadBehaviorConfig(Game* game, lua_State* L, const std::string& path);
	
	private:
		// sprite
		flat::geometry::Vector2 m_spriteOrigin;
		int m_atlasWidth;
		int m_atlasHeight;
		float m_animationFrameDuration;
		std::shared_ptr<const flat::video::Texture> m_atlas;
		
		// physics
		float m_radius;
		float m_speed;
		
		// behavior
		behavior::Behavior* m_behavior;
};

} // entity
} // game

#endif // GAME_MAP_ENTITYTEMPLATE_H



