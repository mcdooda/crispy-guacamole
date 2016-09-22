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
	m_entityTemplate = editorState->getEntityTemplate(game, "sheep");
}

EntityEditorMode::~EntityEditorMode()
{
}

void EntityEditorMode::applyBrush() const
{
	const map::Map& map = m_editorState->getMap();
	const map::Tile* tile = map.getTileIfWalkable(m_brushPosition.x, m_brushPosition.y);
	if (tile != nullptr)
	{
		flat::Vector3 position(m_brushPosition, tile->getZ());
		entity::Entity* entity = m_editorState->spawnEntityAtPosition(m_entityTemplate, position);
		float currentTime = m_game->time->getFrameTime();
		entity->update(currentTime, 0.f);
	}
}

} // editor
} // states
} // game


