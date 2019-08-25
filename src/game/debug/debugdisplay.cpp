#ifdef FLAT_DEBUG

#include "debugdisplay.h"
#include "../map/map.h"
#include "../game.h"
#include "../states/basemapstate.h"

namespace game
{
namespace debug
{

DebugDisplay::DebugDisplay()
{
	// line shader
	m_lineProgram.load("data/shaders/debug/debugline.frag", "data/shaders/debug/debugline.vert");
	m_lineProgramRenderSettings.viewProjectionMatrixUniform = m_lineProgram.getUniform<flat::Matrix4>("vpMatrix");
	m_lineProgramRenderSettings.positionAttribute           = m_lineProgram.getAttribute("position");
	m_lineProgramRenderSettings.colorAttribute              = m_lineProgram.getAttribute("color");

	// text shader
	m_textProgram.load("data/shaders/debug/debugtext.frag", "data/shaders/debug/debugtext.vert");
	m_textProgramRenderSettings.viewProjectionMatrixUniform = m_textProgram.getUniform<flat::Matrix4>("vpMatrix");
	m_textProgramRenderSettings.modelMatrixUniform          = m_textProgram.getUniform<flat::Matrix4>("modelMatrix");
	m_textProgramRenderSettings.textureUniform              = m_textProgram.getUniform<flat::video::Texture>("fontTexture");
	m_textProgramRenderSettings.secondaryColorUniform       = m_textProgram.getUniform<flat::video::Color>("backgroundColor");

	m_textProgramRenderSettings.positionAttribute = m_textProgram.getAttribute("position");
	m_textProgramRenderSettings.uvAttribute       = m_textProgram.getAttribute("uv");
	m_textProgramRenderSettings.colorAttribute    = m_textProgram.getAttribute("color");
}

void DebugDisplay::loadResources(Game& game)
{
	m_font = game.video->getFont("data/misc/fonts/LucidaSansRegular.ttf", 12);

	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	m_transform = baseMapState.getMap().getTransform();
}

void DebugDisplay::add3dLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& color, float lineWidth)
{
	add3dLine(fromPos, toPos, color, color, lineWidth);
}

void DebugDisplay::add3dLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth)
{
	flat::Vector2 from2d = flat::Vector2(m_transform * fromPos);
	flat::Vector2 to2d = flat::Vector2(m_transform * toPos);
	add2dLine(from2d, to2d, fromColor, toColor, lineWidth);
}

void DebugDisplay::add2dLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& color, float lineWidth)
{
	add2dLine(fromPos, toPos, color, color, lineWidth);
}

void DebugDisplay::add2dLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth)
{
	DebugDisplayLine* debugDisplayLine = new DebugDisplayLine(fromPos, toPos, fromColor, toColor, lineWidth);
	m_lineElements.emplace_back(debugDisplayLine);
}

void DebugDisplay::add2dAABB(const flat::AABB2& aabb, const flat::video::Color& color)
{
	add2dLine(aabb.min, flat::Vector2(aabb.max.x, aabb.min.y), color);
	add2dLine(flat::Vector2(aabb.max.x, aabb.min.y), aabb.max, color);
	add2dLine(aabb.max, flat::Vector2(aabb.min.x, aabb.max.y), color);
	add2dLine(flat::Vector2(aabb.min.x, aabb.max.y), aabb.min, color);
}

void DebugDisplay::add3dAABB(const flat::AABB3& aabb, const flat::video::Color& color)
{
	// bottom square
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.min.y, aabb.min.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.max.y, aabb.min.z));
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.max.y, aabb.min.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.max.y, aabb.min.z));
		add2dLine(a, b, color, color);
	}

	// top square
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.min.y, aabb.max.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.min.y, aabb.max.z));
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.min.y, aabb.max.z));
		flat::Vector2 b = flat::Vector2(m_transform * aabb.max);
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * aabb.max);
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.max.y, aabb.max.z));
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.max.y, aabb.max.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.min.y, aabb.max.z));
		add2dLine(a, b, color, color);
	}

	// vertical edges
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.max.y, aabb.min.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.min.x, aabb.max.y, aabb.max.z));
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.min.y, aabb.min.z));
		flat::Vector2 b = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.min.y, aabb.max.z));
		add2dLine(a, b, color, color);
	}
	{
		flat::Vector2 a = flat::Vector2(m_transform * flat::Vector3(aabb.max.x, aabb.max.y, aabb.min.z));
		flat::Vector2 b = flat::Vector2(m_transform * aabb.max);
		add2dLine(a, b, color, color);
	}
}

void DebugDisplay::add3dCircle(const flat::Vector3& center, float radius, const flat::video::Color& color, float lineWidth)
{
	const int numSegments = 32;
	flat::Vector3 previousPoint = center + flat::Vector3(radius, 0.f, 0.f);
	for (int i = 1; i <= numSegments; ++i)
	{
		const float angle = static_cast<float>(i) / numSegments * flat::PI_2;
		flat::Vector3 point = center + flat::Vector3(cos(angle) * radius, sin(angle) * radius, 0.f);
		add3dLine(previousPoint, point, color, lineWidth);
		previousPoint = point;
	}
}

void DebugDisplay::add3dText(const flat::Vector3& pos, const std::string& text, const flat::video::Color& color, const flat::video::Color& backgroundColor)
{
	flat::Vector3 pos2d = m_transform * pos;
	DebugDisplayText* debugDisplayText = new DebugDisplayText(flat::Vector2(pos2d), text, color, backgroundColor, m_font);
	m_textElements.emplace_back(debugDisplayText);
}

void DebugDisplay::drawElements(Game& game, const flat::video::View& view)
{
	drawElementList(game, view, m_lineProgram, m_lineProgramRenderSettings, m_lineElements);
	drawElementList(game, view, m_textProgram, m_textProgramRenderSettings, m_textElements);
}

void DebugDisplay::drawElementList(Game& game, const flat::video::View& view, flat::video::Program& program, flat::render::RenderSettings& renderSettings, std::vector<std::unique_ptr<DebugDisplayElement>>& elements)
{
	program.use(*game.video->window);
	renderSettings.viewProjectionMatrixUniform.set(view.getViewProjectionMatrix());

	for (const std::unique_ptr<DebugDisplayElement>& element : elements)
	{
		element->draw(renderSettings);
	}
	elements.clear();
}

} // debug
} // game

#endif // FLAT_DEBUG

