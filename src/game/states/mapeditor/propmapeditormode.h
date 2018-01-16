#ifndef GAME_STATES_EDITOR_PROPMAPEDITORMODE_H
#define GAME_STATES_EDITOR_PROPMAPEDITORMODE_H

#include "mapeditormode.h"

namespace game
{
namespace map
{
class PropTemplate;
}
namespace states
{
namespace editor
{

class PropMapEditorMode : public MapEditorMode
{
	using Super = MapEditorMode;
	public:
		PropMapEditorMode(Game& game);
		~PropMapEditorMode() override;

		void updateBrushTiles(MapEditorState& mapEditorState) override;
		void applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed) override;
		void applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed) override;

		inline void setPropTemplate(const std::shared_ptr<const map::PropTemplate>& propTemplate) { m_propTemplate = propTemplate; }

	private:
		std::shared_ptr<const map::PropTemplate> m_propTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_PROPMAPEDITORMODE_H


