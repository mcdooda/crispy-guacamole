#ifdef FLAT_DEBUG
#include <sstream>
#endif

#include "interactioncomponent.h"

namespace game
{
namespace entity
{
namespace component
{
namespace interaction
{

const std::string& InteractionComponent::getBehaviorStateName() const
{
	return m_interactionState ? *m_interactionState : getTemplate()->getBehaviorStateName();
}

void InteractionComponent::setInteractionState(const std::string& interactionState)
{
	m_interactionState = std::make_unique<std::string>(interactionState);
}

void InteractionComponent::resetInteractionState()
{
	m_interactionState.reset();
}

#ifdef FLAT_DEBUG
void InteractionComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add3dText(m_owner->getPosition(), getBehaviorStateName().c_str());
}
#endif

} // interaction
} // component
} // entity
} // game


