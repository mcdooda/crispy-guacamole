#ifndef GAME_ENTITY_SPRITE_DESCRIPTION_H
#define GAME_ENTITY_SPRITE_DESCRIPTION_H

#include <map>
#include <string>
#include <flat.h>
#include "animationdescription.h"

namespace game
{
namespace entity
{
namespace sprite
{

class Description
{
	public:
		Description();
		
		inline void setAtlas(std::shared_ptr<const flat::video::Texture> atlas) { m_atlas = atlas; }
		inline std::shared_ptr<const flat::video::Texture> getAtlas() const { return m_atlas; }
		
		inline void setAtlasWidth(int atlasWidth) { m_atlasWidth = atlasWidth; }
		inline int getAtlasWidth() const { return m_atlasWidth; }
		
		inline void setAtlasHeight(int atlasHeight) { m_atlasHeight = atlasHeight; }
		inline int getAtlasHeight() const { return m_atlasHeight; }
		
		void setOrigin(const flat::geometry::Vector2& origin) { m_origin = origin; }
		inline const flat::geometry::Vector2& getOrigin() const { return m_origin; }
		
		void addAnimationDescription(const std::string& animationName, AnimationDescription animationDescription);
		void cacheMandatoryAnimationDescriptions();
		const AnimationDescription* getAnimationDescription(const std::string& animationName) const;
		inline const AnimationDescription* getMoveAnimationDescription() const { return m_moveAnimationDescription; }
		
	private:
		std::shared_ptr<const flat::video::Texture> m_atlas;
		
		int m_atlasWidth;
		int m_atlasHeight;
		flat::geometry::Vector2 m_origin;
		
		std::map<std::string, AnimationDescription> m_animationDescriptions;
		const AnimationDescription* m_moveAnimationDescription;
};

} // sprite
} // entity
} // game

#endif // GAME_ENTITY_SPRITE_DESCRIPTION_H



