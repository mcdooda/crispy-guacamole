#ifndef GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENTTEMPLATE_H

#include <string>
#include <flat.h>
#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace interaction
{

class InteractionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline const std::string& getBehaviorStateName() const { return m_behaviorStateName; }

	private:
		std::string m_behaviorStateName;
};

} // interaction
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENTTEMPLATE_H


