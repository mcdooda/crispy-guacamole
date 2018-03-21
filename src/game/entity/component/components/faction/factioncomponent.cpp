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

} // faction
} // component
} // entity
} // game


