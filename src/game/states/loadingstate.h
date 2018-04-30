#ifndef GAME_STATES_LOADINGSTATE_H
#define GAME_STATES_LOADINGSTATE_H

#include <future>

#include "basestate.h"

namespace game
{
namespace states
{
class BaseMapState;

class LoadingState : public BaseState
{
	using Super = BaseState;
	public:
		LoadingState(std::unique_ptr<BaseMapState>&& stateToLoad);

		void enter(Game& game) override;
		void execute(Game& game) override;
		void exit(Game& game) override;

		inline BaseMapState& getStateToLoad() const { return *m_stateToLoad; }

	protected:
		bool isStateLoaded() const;

	protected:
		std::unique_ptr<BaseMapState> m_stateToLoad;
		std::future<void> m_loadingFuture;
};

} // states
} // game

#endif // GAME_STATES_LOADINGSTATE_H


