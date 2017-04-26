#ifndef GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H
#define GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H

#include <flat.h>
#include <set>
#include "attackcomponenttemplate.h"
#include "../../component.h"
#include "../../../entityhandle.h"
#include "../../../entitythread.h"

namespace game
{
namespace entity
{
namespace component
{
namespace attack
{

class AttackComponent : public ComponentImpl<AttackComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "attack"; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		void attack(float currentTime);

		inline void setAttackTarget(Entity* entity) { m_target = entity; }
		inline Entity* getAttackTarget() { return m_target.getEntity(); }

		bool isInAttackRange(Entity* entity) const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	public:
		flat::Slot<> attackStarted;
		flat::Slot<> attackStopped;

	private:
		void tryBeginAttack(float currentTime);
		void tryEndAttack();

		void beginAttack(float currentTime);
		void updateAttack();
		void endAttack();

		float getAttackRange() const;

		bool isBusyForAttacking() const;

		bool addedToMap(Entity* entity, map::Map* map);

	private:
		EntityHandle m_target;
		EntityThread m_attackThread;
		float m_lastAttackTime;
		bool m_attacking : 1;
};

} // attack
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H



