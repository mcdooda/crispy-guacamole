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
		
		inline float getDepth() const { return m_depth; }
		inline std::uint32_t getTextureHash() const { return m_textureHash; }
		
		inline void computeDepth(float x, float y, float radius) { m_depth = x + y + radius; }
		inline void setTextureHash(std::uint32_t textureHash) { m_textureHash = textureHash; }

		virtual bool isTile() const { return false; }

		virtual float getMapObjectZ() const = 0;
		
	private:
		float m_depth;
		std::uint32_t m_textureHash;
};

} // map
} // game

#endif // GAME_MAP_MAPOBJECT_H



