#ifndef GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H
#define GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H

#include "lifecomponenttemplate.h"
#include "../../component.h"
#include "../../../entitythread.h"

namespace game
{
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
		inline static bool enableInEditor() { return false; }

		void init() override;

		void update(float currentTime, float elapsedTime) override;

		void dealDamage(int damage);

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<int> damageDealt;
		flat::Slot<> spawn;
		flat::Slot<> despawn;

	private:
		void onSpawn();
		void onDespawn();
		
		void checkSpawnDespawnThreadFinished();

	private:
		EntityThread m_spawnDespawnThread;
		int m_health;
		bool m_spawnNextUpdate : 1;
		bool m_spawning : 1;
		bool m_despawning : 1;
};

} // life
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_LIFE_LIFECOMPONENT_H



