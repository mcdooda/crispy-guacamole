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
		DebugDisplay(const map::Map& map);

		void addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& color = flat::video::Color::BLUE);
		void addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor);

		void drawElements(Game& game, const flat::video::View& view);

	private:
		flat::video::Program m_program;
		flat::render::RenderSettings m_programRenderSettings;

		std::vector<std::unique_ptr<DebugDisplayElement>> m_elements;
		const map::Map& m_map;
};

} // debug
} // game

#endif // FLAT_DEBUG

#endif // GAME_DEBUG_DEBUGDISPLAY_H


