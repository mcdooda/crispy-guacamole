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
		
		virtual flat::render::Sprite& getSprite() = 0;
		inline const flat::render::Sprite& getSprite() const { return const_cast<MapObject*>(this)->getSprite(); }

		virtual const flat::render::ProgramSettings& getProgramSettings() const = 0;
		
		inline void setWorldSpaceAABB(const flat::AABB3& worldSpaceAABB) { m_worldSpaceAABB = worldSpaceAABB; }
		inline const flat::AABB3& getWorldSpaceAABB() const { return m_worldSpaceAABB; }

		inline const flat::AABB2& getAABB() const { return m_spriteAABB; }

		void updateRenderHash() const;
		inline std::uint32_t getRenderHash() const { return m_renderHash; }

		virtual bool isTile() const { return false; }
		virtual bool isEntity() const { return false; }
		
	protected:
		flat::AABB3 m_worldSpaceAABB;
		flat::AABB2 m_spriteAABB;
		mutable std::uint32_t m_renderHash;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



