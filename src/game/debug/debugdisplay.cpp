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
	m_lineProgram.load("data/shaders/debugline.frag", "data/shaders/debugline.vert");
	m_lineProgramRenderSettings.viewProjectionMatrixUniform = m_lineProgram.getUniform<flat::Matrix4>("vpMatrix");
	m_lineProgramRenderSettings.positionAttribute = m_lineProgram.getAttribute("position");
	m_lineProgramRenderSettings.colorAttribute = m_lineProgram.getAttribute("color");

	// text shader
	m_textProgram.load("data/shaders/debugtext.frag", "data/shaders/debugtext.vert");
	m_textProgramRenderSettings.viewProjectionMatrixUniform = m_textProgram.getUniform<flat::Matrix4>("vpMatrix");
	m_textProgramRenderSettings.modelMatrixUniform          = m_textProgram.getUniform<flat::Matrix4>("modelMatrix");
	m_textProgramRenderSettings.textureUniform              = m_textProgram.getUniform<flat::video::Texture>("fontTexture");
	m_textProgramRenderSettings.colorUniform                = m_textProgram.getUniform<flat::video::Color>("color");
	m_textProgramRenderSettings.secondaryColorUniform       = m_textProgram.getUniform<flat::video::Color>("backgroundColor");

	m_textProgramRenderSettings.positionAttribute = m_textProgram.getAttribute("position");
	m_textProgramRenderSettings.uvAttribute       = m_textProgram.getAttribute("uv");
}

void DebugDisplay::loadResources(Game& game)
{
	m_font = game.video->getFont("data/misc/fonts/LucidaSansRegular.ttf", 12);

	states::BaseMapState& baseMapState = game.getStateMachine().getState()->to<states::BaseMapState>();
	m_transform = baseMapState.getMap().getTransform();
}

void DebugDisplay::addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& color, float lineWidth)
{
	addLine(fromPos, toPos, color, color, lineWidth);
}

void DebugDisplay::addLine(const flat::Vector3& fromPos, const flat::Vector3& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth)
{
	flat::Vector3 from2d = m_transform * fromPos;
	flat::Vector3 to2d = m_transform * toPos;

	DebugDisplayLine* debugDisplayLine = new DebugDisplayLine(flat::Vector2(from2d), flat::Vector2(to2d), fromColor, toColor, lineWidth);
	m_lineElements.emplace_back(debugDisplayLine);
}

void DebugDisplay::addText(const flat::Vector3& pos, const std::string& text, const flat::video::Color& color, const flat::video::Color& backgroundColor)
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
	program.use(game.video->window);
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

