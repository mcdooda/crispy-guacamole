#ifndef GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H

#include "interactioncomponenttemplate.h"
#include "../../component.h"

namespace game::entity::component::interaction
{

class InteractionComponent : public ComponentImpl<InteractionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "interaction"; }
		inline static const char* getVisualName() { return "Interaction"; }
		inline static ComponentUpdateType getUpdateType() { return ComponentUpdateType::NONE; }

		const std::string& getBehaviorStateName() const;

		void setInteractionState(const std::string& interactionState);
		void resetInteractionState();

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		std::unique_ptr<std::string> m_interactionState;
};

} // game::entity::component::interaction

#endif // GAME_ENTITY_COMPONENT_INTERACTION_INTERACTIONCOMPONENT_H



