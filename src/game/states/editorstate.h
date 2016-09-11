#ifndef GAME_STATES_EDITORSTATE_H
#define GAME_STATES_EDITORSTATE_H

#include "basemapstate.h"
#include "../map/brush/brush.h"

namespace game
{
namespace map
{
class Tile;
}
namespace states
{

class EditorState : public BaseMapState
{
	typedef BaseMapState Super;
	public:
		void enter(flat::state::Agent* agent) override;
		void execute(flat::state::Agent* agent) override;
		
	private:
		void saveOnCtrlS(Game* game);
		void updateBrush(Game* game);
		void displayBrush(Game* game);
		void applyBrush(Game* game);

		void applyDisplacement(float displacement);
		void applyMean(float frameTime);
		void applyZero();

		void eachBrushTile(std::function<void(map::Tile*, float)> func);

	private:
		std::vector<map::Tile*> m_brushTiles;
		flat::Vector2 m_brushPosition;
		std::unique_ptr<map::brush::Brush> m_brush;
};

} // states
} // game

#endif // GAME_STATES_EDITORSTATE_H


