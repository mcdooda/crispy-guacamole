#include "basestate.h"
#include "lua/game.h"
#include "lua/mapeditor.h"
#include "lua/entityeditor.h"
#include "../game.h"
#include "../mod/lua/mod.h"

namespace game
{
namespace states
{

void BaseState::enter(Game& game)
{
	game.video->window->setTitle("Crispy guacamole");
	initTime(game);
	initLua(game);
	initRender(game);
	resetViews(game);
	game.input->clearFrameEvents();
}

void BaseState::execute(Game& game)
{
	const auto& input = game.input;

	if (input->window->isResized())
	{
		resetViews(game);
	}

	if (input->keyboard->isJustPressed(K(ESCAPE)))
	{
		game.stop();
	}

	update(game);

	clearScreen(game);
	draw(game);
}

void BaseState::exit(Game& game)
{
	flat::sharp::ui::RootWidget& rootWidget = *game.ui->root.get();
	rootWidget.clearAll();
}

void BaseState::update(Game& game)
{
	updateUi(game);
}

void BaseState::clearScreen(Game& game)
{
	game.video->setClearColor(flat::video::Color::BLACK);
	game.video->clear();
}

void BaseState::draw(Game& game)
{
	drawUi(game);
}

void BaseState::resetViews(Game & game)
{
	const flat::Vector2 windowSize = game.video->window->getSize();
	game.interfaceView.reset();
	game.interfaceView.move(windowSize / 2.0f);
	game.interfaceView.updateProjection();
}

void BaseState::initTime(Game& game)
{
	m_clock = game.time->newClock();
}

void BaseState::initLua(Game& game)
{
	game.lua->reset(game);
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		states::lua::game::open(L);
		states::lua::mapeditor::open(L);
		states::lua::entityeditor::open(L);
		mod::lua::open(L);
	}
}

void BaseState::updateUi(Game& game)
{
	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	root->update();
}

void BaseState::drawUi(Game& game)
{
	m_uiRender.program.use(*game.video->window);

	m_uiRender.settings.viewProjectionMatrixUniform.set(game.interfaceView.getViewProjectionMatrix());

	m_uiRender.settings.modelMatrixUniform.set(flat::Matrix4());
	m_uiRender.settings.colorUniform.set(flat::video::Color(1.0f, 0.0f, 0.0f, 1.0f));

	flat::sharp::ui::RootWidget* root = game.ui->root.get();
	root->draw(m_uiRender.settings);
}

void BaseState::initRender(Game& game)
{
	m_uiRender.program.load("data/shaders/ui/ui.frag", "data/shaders/ui/ui.vert");

	m_uiRender.settings.textureUniform = m_uiRender.program.getUniform<flat::video::Texture>("objectTexture");
	m_uiRender.settings.textureGivenUniform = m_uiRender.program.getUniform<bool>("textureGiven");
	m_uiRender.settings.colorUniform = m_uiRender.program.getUniform<flat::video::Color>("globalColor");
	m_uiRender.settings.vertexColorGivenUniform = m_uiRender.program.getUniform<bool>("vertexColorGiven");
	m_uiRender.settings.secondaryColorUniform = m_uiRender.program.getUniform<flat::video::Color>("secondaryColor");
	m_uiRender.settings.modelMatrixUniform = m_uiRender.program.getUniform<flat::Matrix4>("modelMatrix");
	m_uiRender.settings.viewProjectionMatrixUniform = m_uiRender.program.getUniform<flat::Matrix4>("vpMatrix");

	m_uiRender.settings.positionAttribute = m_uiRender.program.getAttribute("position");
	m_uiRender.settings.colorAttribute = m_uiRender.program.getAttribute("vertexColor");
	m_uiRender.settings.uvAttribute = m_uiRender.program.getAttribute("uv");
}

} // states
} // game

