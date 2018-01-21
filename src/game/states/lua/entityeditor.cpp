#include "entityeditor.h"
#include "base.h"
#include "../entityeditorstate.h"
#include "../../game.h"
#include "../../entity/lua/entity.h"

namespace game
{
namespace states
{
namespace lua
{
namespace entityeditor
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_createtable(L, 0, 1);
	static const luaL_Reg EntityEditor_lib_f[] = {
		{"newEntity",     l_EntityEditor_newEntity},
		{"openEntity",    l_EntityEditor_openEntity},

		{"entitySpawned", l_EntityEditor_entitySpawned},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, EntityEditor_lib_f, 0);
	lua_setglobal(L, "EntityEditor");

	return 0;
}

int l_EntityEditor_newEntity(lua_State* L)
{
	FLAT_ASSERT_MSG(false, "not implemented");
	return 0;
}

int l_EntityEditor_openEntity(lua_State* L)
{
	const char* modPath = luaL_checkstring(L, 1);
	const char* mapName = luaL_checkstring(L, 2);
	const char* entityName = luaL_checkstring(L, 3);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.modPath = modPath;
	game.mapName = mapName;
	game.entityName = entityName;
	std::unique_ptr<EntityEditorState> gameState = std::make_unique<EntityEditorState>();
	gameState->setModPath(modPath);
	game.getStateMachine().setNextState(std::move(gameState));
	return 1;
}

int l_EntityEditor_entitySpawned(lua_State* L)
{
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onEntitySpawned;
	onEntitySpawned.set(L, 1);

	EntityEditorState& entityEditorState = base::getBaseState(L).to<EntityEditorState>();
	entityEditorState.entitySpawned.on([L, onEntitySpawned](entity::Entity* entity)
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		onEntitySpawned.push(L);
		luaL_checktype(L, -1, LUA_TFUNCTION);
		entity::lua::pushEntity(L, entity);
		lua_call(L, 1, 1);
		luaL_checktype(L, -1, LUA_TBOOLEAN);
		bool keepCallback = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return keepCallback;
	});
	return 0;
}

} // editor
} // lua
} // states
} // game