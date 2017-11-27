#ifndef GAME_STATES_BASESTATE_H
#define GAME_STATES_BASESTATE_H

#include <flat.h>
#include "../timer/timercontainer.h"

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

		// time
		inline const flat::time::Clock& getClock() const { FLAT_ASSERT(m_clock != nullptr); return *m_clock.get(); }
		inline flat::time::Clock& getClock() { FLAT_ASSERT(m_clock != nullptr); return *m_clock.get(); }
		inline timer::TimerContainer& getTimerContainer() { return m_timerContainer; }

	protected:
		virtual void update(Game& game);
		virtual void draw(Game& game);

		void resetViews(Game& game);

		// time
		void initTime(Game& game);
		void updateTimers(Game& game);

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

		// time
		std::shared_ptr<flat::time::Clock> m_clock;
		std::shared_ptr<flat::time::Clock> m_uiClock;
		timer::TimerContainer m_timerContainer;
};

} // states
} // game

#endif // GAME_STATES_BASESTATE_H


