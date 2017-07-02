#include "entitymapeditormode.h"
#include "../mapeditorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

EntityMapEditorMode::EntityMapEditorMode(Game& game) : Super(game)
{
	
}

EntityMapEditorMode::~EntityMapEditorMode()
{
	m_game.getStateMachine().getState()->as<states::MapEditorState>().clearGhostTemplate();
}

void EntityMapEditorMode::applyBrushPrimaryEffect(bool justPressed)
{
	FLAT_ASSERT_MSG(m_entityTemplate != nullptr, "Trying to put an entity without calling Editor.setEntity first");
	if (m_brushOnTile)
	{
		flat::Vector2 position2d = m_brushPosition;
		// add a little noise to avoid getting entities at the exact same position
		position2d.x += m_game.random->nextFloat(-0.001f, 0.001f);
		position2d.y += m_game.random->nextFloat(-0.001f, 0.001f);
		const map::Map& map = getMap();
		const map::Tile* tile = map.getTileIfWalkable(position2d.x, position2d.y);
		if (tile != nullptr)
		{
			flat::Vector3 position(position2d.x, position2d.y, tile->getZ());
			getEditorState().spawnEntityAtPosition(m_game, m_entityTemplate, position);
		}
	}
}

void EntityMapEditorMode::handleShortcuts()
{
	const auto& keyboard = m_game.input->keyboard;
	const bool delPressed = keyboard->isJustPressed(K(DELETE));
	if (delPressed)
	{
		states::MapEditorState& editorState = getEditorState();
		for (entity::Entity* entity : editorState.getSelectedEntities())
		{
			entity->markForDelete();
		}
	}
}

void EntityMapEditorMode::setEntityTemplate(const std::shared_ptr<const entity::EntityTemplate>& entityTemplate)
{
	m_entityTemplate = entityTemplate;
	getEditorState().setGhostTemplate(m_game, entityTemplate);
}

} // editor
} // states
} // game


