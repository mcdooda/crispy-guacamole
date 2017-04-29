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
		
		inline const flat::AABB3& getAABB() const { return m_AABB; }

		std::uint32_t getTextureHash() const;

		virtual bool isTile() const { return false; }
		virtual bool isEntity() const { return false; }
		
	protected:
		flat::AABB3 m_AABB;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



