#ifndef GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H
#define GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H

#include "lifecomponenttemplate.h"
#include "../../component.h"
#include "../../../entitythread.h"

namespace game
{
namespace map
{
class Map;
}
namespace entity
{
namespace component
{
namespace life
{

class LifeComponent : public ComponentImpl<LifeComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "life"; }
		inline static bool enableInMapEditor() { return false; }

		void init(lua_State* L) override;
		void deinit(lua_State* L) override;

		void update(lua_State* L, float currentTime, float elapsedTime) override;

		void kill(lua_State* L);
		void dealDamage(lua_State* L, int damage);

		inline int getHealth() const { return m_health; }
		inline int getMaxHealth() const { return getTemplate()->getMaxHealth(); }

		int addHealthChangedCallback(lua_State* L, int index);
		void pushHealthChangedCallback(lua_State* L, int callbackIndex) const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<int> healthChanged;
		flat::Slot<> live;
		flat::Slot<> die;

	private:
		bool addedToMap(lua_State* L, Entity* entity, map::Map* map);

		void onLive(lua_State* L);
		void onDie(lua_State* L);
		
		void checkSpawnDespawnThreadFinished();

	private:
		std::vector<flat::lua::UniqueLuaReference<LUA_TFUNCTION>> m_healthChangedRefs;
		EntityThread m_spawnDespawnThread;
		int m_health;
		bool m_spawning : 1;
		bool m_despawning : 1;
};

} // life
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H



