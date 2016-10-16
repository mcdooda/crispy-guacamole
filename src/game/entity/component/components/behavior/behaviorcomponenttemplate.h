#ifndef GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"
#include "../../../behavior/behavior.h"

namespace game
{
namespace entity
{
namespace component
{

class BehaviorComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const behavior::Behavior& getBehavior() const { return *m_behavior.get(); }

	private:
		std::unique_ptr<behavior::Behavior> m_behavior;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENTTEMPLATE_H


