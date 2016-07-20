#include "editorstate.h"
#include "../game.h"

namespace game
{
namespace states
{

void EditorState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	flat::lua::doFile(m_luaState, "data/editor/scripts/ui.lua");
	m_ui->fullLayout();
}

} // states
} // game



