#include "entitymapeditormode.h"
#include "../mapeditorstate.h"
#include "../../game.h"
#include "../../entity/entitytemplate.h"
#include "../../entity/component/components/movement/movementcomponent.h"

namespace game
{
namespace states
{
namespace editor
{

EntityMapEditorMode::EntityMapEditorMode(Game& game) : Super(game)
{

}

void EntityMapEditorMode::exit(MapEditorState& mapEditorState)
{
	Super::exit(mapEditorState);
	mapEditorState.clearGhostTemplate();
}

void EntityMapEditorMode::applyBrushPrimaryEffect(MapEditorState& mapEditorState, bool justPressed)
{
	FLAT_ASSERT_MSG(m_entityTemplate != nullptr, "Trying to put an entity without calling Editor.setEntity first");
	if (m_brushOnTile)
	{
		flat::Vector2 position2d = m_brushPosition;
		// add a little noise to avoid getting entities at the exact same position
		position2d.x += m_game.random->nextFloat(-0.001f, 0.001f);
		position2d.y += m_game.random->nextFloat(-0.001f, 0.001f);
		const map::Map& map = mapEditorState.getMap();
		map::Navigability navigabilityMask = map::Navigability::ALL;
		const entity::component::movement::MovementComponentTemplate* movementComponentTemplate = m_entityTemplate->getComponentTemplate<entity::component::movement::MovementComponent>();
		if (movementComponentTemplate != nullptr)
		{
			navigabilityMask = movementComponentTemplate->getNavigabilityMask();
		}
		const map::Tile* tile = map.getTileIfNavigable(position2d.x, position2d.y, navigabilityMask);
		if (tile != nullptr)
		{
			flat::Vector3 position(position2d.x, position2d.y, tile->getZ());
			mapEditorState.spawnEntityAtPosition(m_game, m_entityTemplate, position);
		}
	}
}

void EntityMapEditorMode::handleShortcuts(MapEditorState& mapEditorState)
{
	const auto& keyboard = mapEditorState.m_gameInputContext->getKeyboardInputContext();
	const bool delPressed = keyboard.isJustPressed(K(DELETE));
	if (delPressed)
	{
		for (entity::Entity* entity : mapEditorState.getSelectedEntities())
		{
			entity->markForDelete();
		}
	}
}

void EntityMapEditorMode::setEntityTemplate(MapEditorState& mapEditorState, const std::shared_ptr<const entity::EntityTemplate>& entityTemplate)
{
	m_entityTemplate = entityTemplate;
	mapEditorState.setGhostTemplate(m_game, entityTemplate);
}

} // editor
} // states
} // game


