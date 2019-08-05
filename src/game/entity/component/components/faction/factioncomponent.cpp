#include <iostream>
#include "factioncomponent.h"
#include "../../../faction/faction.h"

namespace game
{
namespace entity
{
namespace component
{
namespace faction
{

const entity::faction::Faction& FactionComponent::getFaction() const
{
	return getTemplate()->getFaction();
}

#ifdef FLAT_DEBUG
void FactionComponent::debugDraw(debug::DebugDisplay& debugDisplay) const
{
	debugDisplay.add3dText(m_owner->getPosition(), getTemplate()->getFaction().getName());
}
#endif

} // faction
} // component
} // entity
} // game


