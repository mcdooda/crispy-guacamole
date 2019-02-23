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

void PlayerControllerComponent::update(float time, float dt)
{
	getTemplate()->getUpdate().call(m_owner);
}

} // playercontroller
} // component
} // entity
} // game



