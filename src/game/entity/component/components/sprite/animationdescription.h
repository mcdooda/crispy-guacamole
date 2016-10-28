#ifndef GAME_ENTITY_COMPONENT_SPRITE_ANIMATIONDESCRIPTION_H
#define GAME_ENTITY_COMPONENT_SPRITE_ANIMATIONDESCRIPTION_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

class AnimationDescription
{
	public:
		AnimationDescription(int line, int numFrames, float frameDuration);
		
		int getLine() const { return m_line; }
		int getNumFrames() const { return m_numFrames; }
		float getFrameDuration() const { return m_frameDuration; }
		
		FLAT_DEBUG_ONLY(
			inline void setName(const std::string& name) { m_name = name; }
			inline const std::string& getName() const { return m_name; }
		)
		
	private:
		int m_line;
		int m_numFrames;
		float m_frameDuration;
		FLAT_DEBUG_ONLY(std::string m_name);
};

} // sprite
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITE_ANIMATIONDESCRIPTION_H



