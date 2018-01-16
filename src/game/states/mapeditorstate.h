#ifndef GAME_STATES_MAPEDITORSTATE_H
#define GAME_STATES_MAPEDITORSTATE_H

#include "basemapstate.h"
#include "../map/brush/brush.h"
#include "../map/editormap.h"

namespace game
{
namespace map
{
class Tile;
}
namespace states
{
namespace editor
{
class MapEditorMode;
}

class MapEditorState : public BaseMapStateImpl<map::EditorMap>, public flat::state::Agent
{
	friend class editor::MapEditorMode;
	using Super = BaseMapStateImpl<map::EditorMap>;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;

		editor::MapEditorMode* getEditorMode();
		void setEditorMode(std::unique_ptr<editor::MapEditorMode>&& editorMode);

	protected:
		entity::component::ComponentFlags getComponentsFilter() const override;
		
	private:
		void saveOnCtrlS(Game& game);
};

} // states
} // game

#endif // GAME_STATES_MAPEDITORSTATE_H


