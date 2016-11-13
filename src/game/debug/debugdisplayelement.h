#ifndef GAME_DEBUG_DEBUGDISPLAYELEMENT_H
#define GAME_DEBUG_DEBUGDISPLAYELEMENT_H

#ifdef FLAT_DEBUG

#include <flat.h>

namespace game
{
namespace debug
{

struct DebugDisplayVertex
{
	flat::Vector2 pos;
	flat::video::Color color;
};

class DebugDisplayElement
{
	public:
		DebugDisplayElement();
		virtual ~DebugDisplayElement();
		virtual void draw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const = 0;
};

class DebugDisplayLine : public DebugDisplayElement
{
	public:
		DebugDisplayLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth);

		void draw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const override;

	private:
		static const int NUM_VERTICES = 2;
		DebugDisplayVertex m_vertices[NUM_VERTICES];
		float m_lineWidth;
};

} // debug
} // game

#endif // FLAT_DEBUG

#endif // GAME_DEBUG_DEBUGDISPLAYELEMENT_H


