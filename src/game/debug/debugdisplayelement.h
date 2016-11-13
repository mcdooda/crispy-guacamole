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
		virtual void draw(const flat::render::RenderSettings& renderSettings) const = 0;
};

class DebugDisplayLine : public DebugDisplayElement
{
	public:
		DebugDisplayLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth);

		void draw(const flat::render::RenderSettings& renderSettings) const override;

	private:
		static const int NUM_VERTICES = 2;
		DebugDisplayVertex m_vertices[NUM_VERTICES];
		float m_lineWidth;
};

class DebugDisplayText : public DebugDisplayElement, flat::video::font::String
{
public:
	DebugDisplayText(const flat::Vector2& pos, const std::string& text, const flat::video::Color& color, const flat::video::Color& backgroundColor, const std::shared_ptr<const flat::video::font::Font>& font);

	void draw(const flat::render::RenderSettings& renderSettings) const override;

private:
	flat::Matrix4 m_transform;
	flat::video::Color m_color;
	flat::video::Color m_backgroundColor;
};

} // debug
} // game

#endif // FLAT_DEBUG

#endif // GAME_DEBUG_DEBUGDISPLAYELEMENT_H


