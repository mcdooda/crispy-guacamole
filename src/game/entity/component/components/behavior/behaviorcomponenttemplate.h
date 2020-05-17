#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENTTEMPLATE_H

#include "behavior.h"
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace behavior
{

class BehaviorComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline const Behavior& getBehavior() const { return *m_behavior.get(); }

	private:
		std::unique_ptr<Behavior> m_behavior;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENTTEMPLATE_H


