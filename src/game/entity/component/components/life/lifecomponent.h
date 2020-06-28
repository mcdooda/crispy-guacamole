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
class EntityUpdater;

namespace component
{
namespace life
{

class LifeComponent : public ComponentImpl<LifeComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "life"; }
		inline static const char* getVisualName() { return "Life"; }
		inline static bool enableInMapEditor() { return false; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		void kill();
		void dealDamage(int damage, Entity* instigator = nullptr);

		void setHealth(int health);
		inline int getHealth() const { return m_health; }
		inline int getMaxHealth() const { return getTemplate()->getMaxHealth(); }

		int addHealthChangedCallback(lua_State* L, int index);
		void removeHealthChangeCallback(int index);

		int addDamageTakenCallback(lua_State* L, int index);
		void removeDamageTakenCallback(int index);

		int addDiedCallback(lua_State* L, int index);
		void removeDiedCallback(int index);

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<int> healthChanged;
		flat::Slot<int, Entity*> damageTaken;
		flat::Slot<> live;
		flat::Slot<> die;

	private:
		bool addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);

		void onLive();
		void onDie();
		
		void checkSpawnDespawnThreadFinished();

	private:
		flat::lua::SlotProxy<int> m_healthChangedSlotProxy;
		flat::lua::SlotProxy<int, Entity*> m_damageTakenSlotProxy;
		flat::lua::SlotProxy<> m_diedSlotProxy;
		EntityThread m_spawnDespawnThread;
		int m_health;
		bool m_spawning : 1;
		bool m_despawning : 1;

#ifdef FLAT_DEBUG
		bool m_dead : 1;
#endif
};

} // life
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H



