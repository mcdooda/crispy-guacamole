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
	using Super = MapEditorMode;
public:
	EntityMapEditorMode(Game& game);

	void exit(MapEditorState& mapEditorState) override;

	void applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed) override;
	void handleShortcuts(MapEditorState& mapEditorState) override;
	bool canSelectEntities() const override { return true; }

	void setEntityTemplate(MapEditorState& mapEditorState, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate);

private:
	std::shared_ptr<const entity::EntityTemplate> m_entityTemplate;
};

} // editor
} // states
} // game

#endif // GAME_STATES_EDITOR_ENTITYMAPEDITORMODE_H


