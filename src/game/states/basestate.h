#ifndef GAME_STATES_BASESTATE_H
#define GAME_STATES_BASESTATE_H

#include <flat.h>

namespace game
{
class Game;
namespace states
{

class BaseState : public flat::state::StateImpl<Game>
{
	public:
		void enter(Game& game) override;
		void execute(Game& game) override;
		void exit(Game& game) override;

	protected:
		virtual void update(Game& game);

		void clearScreen(Game& game);
		virtual void draw(Game& game);

		void resetViews(Game& game);

		// lua
		void initLua(Game& game);

		// ui
		void updateUi(Game& game);
		void drawUi(Game& game);

		// rendering
		void initRender(Game& game);


	protected:
		// rendering
		flat::render::ProgramSettings m_uiRender;
};

} // states
} // game

#endif // GAME_STATES_BASESTATE_H


