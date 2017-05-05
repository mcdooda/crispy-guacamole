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
		MapObject() {}
		virtual ~MapObject() {}
		
		virtual const flat::render::Sprite& getSprite() const = 0;
		
		inline const flat::AABB3& getWorldSpaceAABB() const { return m_worldSpaceAABB; }
		inline const flat::AABB2& getAABB() const { return m_spriteAABB; }

		std::uint32_t getTextureHash() const;

		virtual bool isTile() const { return false; }
		virtual bool isEntity() const { return false; }
		
	protected:
		flat::AABB3 m_worldSpaceAABB;
		flat::AABB2 m_spriteAABB;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



