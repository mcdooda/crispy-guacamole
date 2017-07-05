#ifndef GAME_STATES_ENTITYEDITORSTATE_H
#define GAME_STATES_ENTITYEDITORSTATE_H

#include "basemapstate.h"
#include "../map/editormap.h"

namespace game
{
namespace states
{

class EntityEditorState : public BaseMapStateImpl<map::EditorMap>
{
	typedef BaseMapStateImpl<map::EditorMap> Super;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;

	protected:
		entity::component::ComponentFlags getComponentsFilter() const override;

	private:
		void spawnEntity(Game& game);

	private:
		entity::EntityHandle m_entity;
};

} // states
} // game

#endif // GAME_STATES_ENTITYEDITORSTATE_H


