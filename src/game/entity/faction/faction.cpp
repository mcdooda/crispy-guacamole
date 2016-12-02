#include "faction.h"

namespace game
{
namespace entity
{
namespace faction
{

Faction::Faction(const std::string& name) :
	m_name(name)
{
	
}

void Faction::addFactionRelation(const Faction& faction, FactionRelation relation)
{
	m_factionRelations[&faction] = relation;
}

FactionRelation Faction::getFactionRelation(const Faction& other) const
{
	std::map<const Faction*, FactionRelation>::const_iterator it = m_factionRelations.find(&other);
	if (it == m_factionRelations.end())
	{
		return FactionRelation::NEUTRAL;
	}
	return it->second;
}

} // faction
} // entity
} // game


