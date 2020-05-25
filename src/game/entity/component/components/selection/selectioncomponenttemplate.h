#ifndef GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENTTEMPLATE_H

#include <flat.h>
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace selection
{

class SelectionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline bool canBeSelected() const { return m_canBeSelected; }

	private:
		bool m_canBeSelected;
};

} // selection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SELECTION_SELECTIONCOMPONENTTEMPLATE_H


