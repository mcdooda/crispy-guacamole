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
		{"openEntity",      l_EntityEditor_openEntity},

		{"entitySpawned",   l_EntityEditor_entitySpawned},
		{"entityDespawned", l_EntityEditor_entityDespawned},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, EntityEditor_lib_f, 0);
	lua_setglobal(L, "EntityEditor");

	return 0;
}

int l_EntityEditor_openEntity(lua_State* L)
{
	const char* modPath = luaL_checkstring(L, 1);
	const char* mapPath = luaL_checkstring(L, 2);
	const char* entityPath = luaL_checkstring(L, 3);
	Game& game = flat::lua::getFlatAs<Game>(L);
	game.mod.setPath(modPath);
	game.mapPath = mapPath;
	game.entityPath = entityPath;
	std::unique_ptr<EntityEditorState> gameState = std::make_unique<EntityEditorState>();
	game.getStateMachine().setNextState(std::move(gameState));
	return 1;
}

int l_EntityEditor_entitySpawned(lua_State* L)
{
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onEntitySpawned(L, 1);

	EntityEditorState& entityEditorState = base::getBaseState(L).to<EntityEditorState>();
	entityEditorState.entitySpawned.on([L, onEntitySpawned](entity::Entity* entity)
	{
		bool keepCallback = true;
		onEntitySpawned.callFunction(
			[entity](lua_State* L)
			{
				entity::lua::pushEntity(L, entity);
			},
			1,
			[&keepCallback](lua_State* L)
			{
				keepCallback = lua_toboolean(L, -1) == 1;
			}
		);
		return keepCallback;
	});
	return 0;
}

int l_EntityEditor_entityDespawned(lua_State* L)
{
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onEntityDespawned;
	onEntityDespawned.set(L, 1);

	EntityEditorState& entityEditorState = base::getBaseState(L).to<EntityEditorState>();
	entityEditorState.entityDespawned.on([L, onEntityDespawned]()
	{
		bool keepCallback = true;
		onEntityDespawned.callFunction(
			[](lua_State* L)
			{
			},
			1,
			[&keepCallback](lua_State* L)
			{
				keepCallback = lua_toboolean(L, -1) == 1;
			}
		);
		return keepCallback;
	});
	return 0;
}

} // editor
} // lua
} // states
} // game