#include "playercontrollercomponent.h"
#include "../../../lua/entity.h"

namespace game
{
namespace entity
{
namespace component
{
namespace playercontroller
{

void PlayerControllerComponent::init()
{
	m_gamepadIndex = 0;
}

void PlayerControllerComponent::update(float time, float dt)
{
	getTemplate()->getUpdate().call(m_owner);
}

} // playercontroller
} // component
} // entity
} // game



