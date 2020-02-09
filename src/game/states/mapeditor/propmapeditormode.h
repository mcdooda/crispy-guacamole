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

		void enter(MapEditorState& mapEditorState) override;

		void updateBrushTiles(MapEditorState& mapEditorState) override;
		void applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed) override;
		void applyBrushSecondaryEffect(MapEditorState& mapEditorState, bool justPressed) override;

		inline void setPropTemplate(const std::shared_ptr<const map::PropTemplate>& propTemplate) { m_propTemplate = propTemplate; }

		void preDraw(Game& game) override;
		void postDraw(Game& game) override;

	private:
		std::shared_ptr<const map::PropTemplate> m_propTemplate;

		std::vector<map::Prop> m_temporaryProps;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_PROPMAPEDITORMODE_H


