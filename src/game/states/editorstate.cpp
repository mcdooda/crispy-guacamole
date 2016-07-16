#include "editorstate.h"

namespace game
{
namespace states
{

void EditorState::enter(flat::state::Agent* agent)
{
	Super::enter(agent);
	
	flat::lua::doFile(m_luaState, "data/editor/scripts/ui.lua");
}

} // states
} // game



