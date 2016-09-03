#include <flat.h>
#include "component.h"

namespace game
{
namespace entity
{
namespace component
{

Component::Component()
{

}

Component::~Component()
{
	
}

void Component::setOwner(Entity* owner)
{
	m_owner = owner;
}

} // component
} // entity
} // game



