#ifndef GAME_STATES_EDITOR_ENTITYEDITORMODE_H
#define GAME_STATES_EDITOR_ENTITYEDITORMODE_H

#include "editormode.h"
#include "../../entity/entitytemplate.h"

namespace game
{
namespace states
{
namespace editor
{

class EntityEditorMode : public EditorMode
{
	typedef EditorMode Super;
public:
	EntityEditorMode(Game& game);
	~EntityEditorMode() override;

	void applyBrushPrimaryEffect(bool justPressed) override;
	void handleShortcuts() override;
	bool canSelectEntities() const override { return true; }

	void setEntityTemplate(const std::shared_ptr<const entity::EntityTemplate>& entityTemplate);

private:
	std::shared_ptr<const entity::EntityTemplate> m_entityTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_ENTITYEDITORMODE_H


