#ifndef GAME_STATES_EDITOR_PROPEDITORMODE_H
#define GAME_STATES_EDITOR_PROPEDITORMODE_H

#include "editormode.h"

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

class PropEditorMode : public EditorMode
{
	typedef EditorMode Super;
	public:
		PropEditorMode(Game& game, EditorState& editorState);
		~PropEditorMode() override;

		void updateBrushTiles() override;
		void displayBrush() const override;
		void applyBrushPrimaryEffect(bool justPressed) const override;
		void applyBrushSecondaryEffect(bool justPressed) const override;

		inline void setPropTemplate(const std::shared_ptr<const map::PropTemplate>& propTemplate) { m_propTemplate = propTemplate; }

	private:
		std::shared_ptr<const map::PropTemplate> m_propTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_PROPEDITORMODE_H


