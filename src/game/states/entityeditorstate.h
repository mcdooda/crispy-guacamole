#ifndef GAME_STATES_ENTITYEDITORSTATE_H
#define GAME_STATES_ENTITYEDITORSTATE_H

#include "basemapstate.h"
#include "../map/map.h"

namespace game
{
namespace states
{

class EntityEditorState : public BaseMapStateImpl<map::Map>
{
	using Super = BaseMapStateImpl<map::Map>;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;

	public:
		flat::Slot<entity::Entity*> entitySpawned;
		flat::Slot<> entityDespawned;

	private:
		void spawnEntity(Game& game);
		void respawnEntityIfNeeded(Game& game);

#ifdef FLAT_DEBUG
		void updateDebuggedComponent();
#endif

	private:
		entity::EntityHandle m_entity;
#ifdef FLAT_DEBUG
		entity::component::ComponentFlags m_debuggedComponentFlags;
#endif
};

} // states
} // game

#endif // GAME_STATES_ENTITYEDITORSTATE_H


