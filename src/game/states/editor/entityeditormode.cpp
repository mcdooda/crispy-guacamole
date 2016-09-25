#include "entityeditormode.h"
#include "../editorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

EntityEditorMode::EntityEditorMode(Game* game, EditorState* editorState) : Super(game, editorState)
{
	
}

EntityEditorMode::~EntityEditorMode()
{

}

void EntityEditorMode::applyBrushPrimaryEffect() const
{
	FLAT_ASSERT_MSG(m_entityTemplate != nullptr, "Trying to put an entity without calling Editor.setEntity first");
	const map::Map& map = m_editorState->getMap();
	const map::Tile* tile = map.getTileIfWalkable(m_brushPosition.x, m_brushPosition.y);
	if (tile != nullptr)
	{
		flat::Vector3 position(m_brushPosition, tile->getZ());
		entity::Entity* entity = m_editorState->spawnEntityAtPosition(m_entityTemplate, position);
		//float currentTime = m_game->time->getFrameTime();
		//entity->update(currentTime, 0.f);
	}
}

} // editor
} // states
} // game


