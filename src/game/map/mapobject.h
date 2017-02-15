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
		MapObject() : m_textureHash(0) {}
		virtual ~MapObject() {}
		
		virtual const flat::render::Sprite& getSprite() const = 0;
		
		inline const flat::AABB3& getAABB() const { return m_AABB; }

		inline void setTextureHash(std::uint32_t textureHash) { m_textureHash = textureHash; }
		inline std::uint32_t getTextureHash() const { return m_textureHash; }

		virtual bool isTile() const { return false; }
		virtual bool isEntity() const { return false; }
		
	protected:
		flat::AABB3 m_AABB;

	private:
		std::uint32_t m_textureHash;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



