#include "entityeditormode.h"
#include "../editorstate.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

EntityEditorMode::EntityEditorMode(Game& game) : Super(game)
{
	
}

EntityEditorMode::~EntityEditorMode()
{

}

void EntityEditorMode::applyBrushPrimaryEffect(bool justPressed) const
{
	FLAT_ASSERT_MSG(m_entityTemplate != nullptr, "Trying to put an entity without calling Editor.setEntity first");
	flat::Vector2 position = m_brushPosition;
	// add a little noise to avoid getting entities at the exact same position
	position.x += m_game.random->nextFloat(-0.001f, 0.001f);
	position.y += m_game.random->nextFloat(-0.001f, 0.001f);
	const map::Map& map = getMap();
	const map::Tile* tile = map.getTileIfWalkable(position.x, position.y);
	if (tile != nullptr)
	{
		flat::Vector3 position(position.x, position.y, tile->getZ());
		getEditorState().spawnEntityAtPosition(m_game, m_entityTemplate, position, 0.f, 0.f);
	}
}

void EntityEditorMode::handleShortcuts() const
{
	const flat::input::Keyboard* keyboard = m_game.input->keyboard;
	const bool delPressed = keyboard->isJustPressed(K(DELETE));
	if (delPressed)
	{
		states::EditorState& editorState = getEditorState();
		for (entity::Entity* entity : editorState.getSelectedEntities())
		{
			editorState.markEntityForDelete(entity);
		}
	}
}

} // editor
} // states
} // game


