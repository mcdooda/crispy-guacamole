#ifndef GAME_ENTITY_COMPONENT_FACTION_FACTION_H
#define GAME_ENTITY_COMPONENT_FACTION_FACTION_H

#include <string>
#include <map>
#include <memory>
#include "factionrelation.h"

namespace game
{
namespace entity
{
namespace faction
{

class Faction
{
	public:
		Faction(const std::string& name);

		inline void setName(const std::string& name) { m_name = name; }
		inline const std::string& getName() const { return m_name; }

		void addFactionRelation(const Faction& faction, FactionRelation relation);
		FactionRelation getFactionRelation(const Faction& other) const;

	private:
		std::string m_name;
		std::map<const Faction*, FactionRelation> m_factionRelations;
};

} // faction
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_FACTION_FACTION_H


