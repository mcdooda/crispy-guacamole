#include "component.h"

namespace game
{
namespace entity
{
namespace component
{

void Component::setOwner(Entity* owner)
{
	m_owner = owner;
}

} // component
} // entity
} // game



