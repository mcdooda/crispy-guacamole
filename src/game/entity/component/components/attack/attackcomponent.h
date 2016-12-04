#ifndef GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H
#define GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H

#include <flat.h>
#include <set>
#include "attackcomponenttemplate.h"
#include "../../component.h"
#include "../../../entityhandle.h"

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

		void update(float currentTime, float elapsedTime) override;

		void attack(float currentTime);

		inline void setAttackTarget(Entity* entity) { m_target = entity; }
		inline Entity* getAttackTarget() { return m_target.getEntity(); }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		EntityHandle m_target;
		float m_lastAttackTime;
};

} // attack
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_ATTACK_ATTACKCOMPONENT_H



