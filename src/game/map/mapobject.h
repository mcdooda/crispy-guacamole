#ifndef GAME_MAP_MAPOBJECT_H
#define GAME_MAP_MAPOBJECT_H

#include <flat.h>

namespace game
{
namespace map
{

class MapObject
{
	public:
		MapObject();
		virtual ~MapObject();
		
		virtual void draw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const = 0;
		
		inline float getDepth() const { return m_depth; }
		
		virtual void computeDepth(float x, float y, float radius);
		
	private:
		float m_depth;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



