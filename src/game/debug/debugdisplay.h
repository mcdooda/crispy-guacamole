#ifndef GAME_DEBUG_DEBUGDISPLAY_H
#define GAME_DEBUG_DEBUGDISPLAY_H

#ifdef FLAT_DEBUG

#include <flat.h>
#include <vector>
#include <memory>
#include "debugdisplayelement.h"

namespace game
{
class Game;

namespace map
{
class Map;
}

namespace debug
{

class DebugDisplay
{
	public:
		DebugDisplay();

		void loadResources(Game& game);

		void add3dLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& color = flat::video::Color::BLUE, float lineWidth = 1.f);
		void add3dLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth = 1.f);

		void add2dLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& color = flat::video::Color::BLUE, float lineWidth = 1.f);
		void add2dLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth = 1.f);

		void add2dAABB(const flat::AABB2& aabb, const flat::video::Color& color = flat::video::Color::BLUE);
		void add3dAABB(const flat::AABB3& aabb, const flat::video::Color& color = flat::video::Color::BLUE);

		void add3dCircle(const flat::Vector3& center, float radius, const flat::video::Color& color, float lineWidth = 1.f);

		void add3dText(const flat::Vector3& pos, const std::string& text, const flat::video::Color& color = flat::video::Color::BLACK, const flat::video::Color& backgroundColor = flat::video::Color::WHITE);

		void drawElements(Game& game, const flat::video::View& view);

	private:
		void drawElementList(Game& game, const flat::video::View& view, flat::video::Program& program, flat::render::RenderSettings& renderSettings, std::vector<std::unique_ptr<DebugDisplayElement>>& elements);

	private:
		flat::video::Program m_lineProgram;
		flat::render::RenderSettings m_lineProgramRenderSettings;

		flat::video::Program m_textProgram;
		flat::render::RenderSettings m_textProgramRenderSettings;

		std::shared_ptr<const flat::video::font::Font> m_font;
		flat::Matrix3 m_transform;

		std::vector<std::unique_ptr<DebugDisplayElement>> m_lineElements;
		std::vector<std::unique_ptr<DebugDisplayElement>> m_textElements;
};

} // debug
} // game

#endif // FLAT_DEBUG

#endif // GAME_DEBUG_DEBUGDISPLAY_H


