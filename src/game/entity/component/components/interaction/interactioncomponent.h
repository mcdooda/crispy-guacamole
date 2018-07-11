#ifndef GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H

#include "interactioncomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace interaction
{

class InteractionComponent : public ComponentImpl<InteractionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "interaction"; }
		inline static bool requiresUpdate() { return false; }

		const std::string& getBehaviorStateName() const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)
};

} // interaction
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H



