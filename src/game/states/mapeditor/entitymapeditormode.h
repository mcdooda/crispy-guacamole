#ifndef GAME_STATES_EDITOR_ENTITYMAPEDITORMODE_H
#define GAME_STATES_EDITOR_ENTITYMAPEDITORMODE_H

#include "mapeditormode.h"
#include "../../entity/entitytemplate.h"

namespace game
{
namespace states
{
namespace editor
{

class EntityMapEditorMode : public MapEditorMode
{
	typedef MapEditorMode Super;
public:
	EntityMapEditorMode(Game& game);
	~EntityMapEditorMode() override;

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

#endif // GAME_STATES_EDITOR_ENTITYMAPEDITORMODE_H


