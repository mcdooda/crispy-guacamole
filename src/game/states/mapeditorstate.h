#ifndef GAME_STATES_MAPEDITORSTATE_H
#define GAME_STATES_MAPEDITORSTATE_H

#include "basemapstate.h"
#include "editor/editormode.h"
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

class MapEditorState : public BaseMapStateImpl<map::EditorMap>
{
	typedef BaseMapStateImpl<map::EditorMap> Super;
	public:
		void enter(Game& game) override final;
		void execute(Game& game) override final;

		inline void setEditorMode(editor::EditorMode* editorMode) { m_editorMode.reset(editorMode); }
		inline editor::EditorMode* getEditorMode() { return m_editorMode.get(); }

	protected:
		entity::component::ComponentFlags getComponentsFilter() const override;
		
	private:
		void saveOnCtrlS(Game& game);
		void updateBrush(Game& game);
		void clearBrush(Game& game);
		void displayBrush(Game& game);
		void applyBrush(Game& game);

	private:
		std::unique_ptr<editor::EditorMode> m_editorMode;
};

} // states
} // game

#endif // GAME_STATES_MAPEDITORSTATE_H


