#ifdef FLAT_DEBUG

#include "debugdisplay.h"
#include "../map/map.h"
#include "../game.h"

namespace game
{
namespace debug
{

DebugDisplay::DebugDisplay(const map::Map& map) :
	m_map(map)
{
	m_program.load("data/shaders/debug.frag", "data/shaders/debug.vert");

	m_programRenderSettings.viewProjectionMatrixUniform = m_program.getUniform<flat::Matrix4>("vpMatrix");

	m_programRenderSettings.positionAttribute = m_program.getAttribute("position");
	m_programRenderSettings.colorAttribute = m_program.getAttribute("color");
}

void DebugDisplay::addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& color, float lineWidth)
{
	addLine(fromPos, toPos, color, color, lineWidth);
}

void DebugDisplay::addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth)
{
	const flat::Matrix3& mapTransform = m_map.getTransform();

	flat::Vector3 from2d = mapTransform * fromPos;
	flat::Vector3 to2d = mapTransform * toPos;

	DebugDisplayLine* line = new DebugDisplayLine(flat::Vector2(from2d), flat::Vector2(to2d), fromColor, toColor, lineWidth);
	m_elements.emplace_back(line);
}

void DebugDisplay::drawElements(Game& game, const flat::video::View& view)
{
	m_program.use(game.video->window);
	m_programRenderSettings.viewProjectionMatrixUniform.set(view.getViewProjectionMatrix());

	const flat::Matrix4& viewMatrix = view.getViewMatrix();

	for (const std::unique_ptr<DebugDisplayElement>& element : m_elements)
	{
		element->draw(m_programRenderSettings, viewMatrix);
	}
	m_elements.clear();
}

} // debug
} // game

#endif // FLAT_DEBUG

