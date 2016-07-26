#ifndef GAME_MAP_ENTITYTEMPLATE_H
#define GAME_MAP_ENTITYTEMPLATE_H

#include <memory>
#include <flat.h>

namespace game
{
namespace map
{

class EntityTemplate final
{
	public:
		EntityTemplate();
		~EntityTemplate();
		
		inline void setSpriteOrigin(const flat::geometry::Vector2& spriteOrigin) { m_spriteOrigin = spriteOrigin; }
		inline const flat::geometry::Vector2& getSpriteOrigin() const { return m_spriteOrigin; }
		
		inline void setAtlas(std::shared_ptr<const flat::video::Texture> atlas) { m_atlas = atlas; }
		inline std::shared_ptr<const flat::video::Texture> getAtlas() const { return m_atlas; }
		
		inline void setAtlasWidth(int atlasWidth) { m_atlasWidth = atlasWidth; }
		inline int getAtlasWidth() const { return m_atlasWidth; }
		
		inline void setAtlasHeight(int atlasHeight) { m_atlasHeight = atlasHeight; }
		inline int getAtlasHeight() const { return m_atlasHeight; }
		
		inline void setAnimationFrameDuration(float animationFrameDuration) { m_animationFrameDuration = animationFrameDuration; }
		inline float getAnimationFrameDuration() const { return m_animationFrameDuration; }
		
		inline void setRadius(float radius) { m_radius = radius; }
		inline float getRadius() const { return m_radius; }
		
	private:
		flat::geometry::Vector2 m_spriteOrigin;
		std::shared_ptr<const flat::video::Texture> m_atlas;
		int m_atlasWidth;
		int m_atlasHeight;
		float m_animationFrameDuration;
		float m_radius;
};

} // map
} // game

#endif // GAME_MAP_ENTITYTEMPLATE_H



