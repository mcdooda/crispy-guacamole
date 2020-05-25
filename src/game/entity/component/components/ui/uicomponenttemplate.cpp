#include "uicomponenttemplate.h"
#include "../../../../game.h"

namespace game
{
namespace entity
{
namespace component
{
namespace ui
{

void UiComponentTemplate::load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	m_widgetFactory = &game.ui->factory;

	lua_getfield(L, -1, "addedToMap");
	m_addedToMap.setIfNotNil(L, -1);

	lua_getfield(L, -2, "removedFromMap");
	m_removedFromMap.setIfNotNil(L, -1);

	lua_getfield(L, -3, "selected");
	m_selected.setIfNotNil(L, -1);

	lua_getfield(L, -4, "deselected");
	m_deselected.setIfNotNil(L, -1);

	lua_pop(L, 4);
}

} // ui
} // component
} // entity
} // game


