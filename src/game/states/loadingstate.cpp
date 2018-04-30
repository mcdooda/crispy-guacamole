#include "loadingstate.h"

#include "basemapstate.h"

#include "../game.h"

namespace game
{
namespace states
{

LoadingState::LoadingState(std::unique_ptr<BaseMapState>&& stateToLoad) :
	m_stateToLoad(std::move(stateToLoad))
{
}

void LoadingState::enter(Game& game)
{
	Super::enter(game);

	FLAT_ASSERT(m_stateToLoad != nullptr);
	m_stateToLoad->prepareLoading(game);
	m_loadingFuture = std::async(
		std::launch::async,
		[this, &game]()
		{
			m_stateToLoad->startLoading(game);
		}
	);
}

void LoadingState::execute(Game& game)
{
	Super::execute(game);

	if (isStateLoaded())
	{
		game.getStateMachine().setNextState(std::move(m_stateToLoad));
	}
}

void LoadingState::exit(Game& game)
{
	m_loadingFuture.wait();
	Super::exit(game);
}

bool LoadingState::isStateLoaded() const
{
	return m_loadingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

} // states
} // game
