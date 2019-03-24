#ifndef GAME_STATES_MAPEDITORSTATE_H
#define GAME_STATES_MAPEDITORSTATE_H

#include "basemapstate.h"
#include "../map/brush/brush.h"
#include "../map/map.h"

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
class EntityMapEditorMode;
class PropMapEditorMode;
class TileMapEditorMode;
class ZoneMapEditorMode;
}

class MapEditorState : public BaseMapStateImpl<map::Map>, public flat::state::Agent
{
	friend class editor::MapEditorMode;
	friend class editor::EntityMapEditorMode;
	friend class editor::PropMapEditorMode;
	friend class editor::TileMapEditorMode;
	friend class editor::ZoneMapEditorMode;
	using Super = BaseMapStateImpl<map::Map>;
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


