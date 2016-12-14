#ifndef GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"
#include "../../../faction/faction.h"

namespace game
{
namespace entity
{
namespace component
{
namespace faction
{

class FactionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const entity::faction::Faction& getFaction() const { return *m_faction; }

	private:
		const entity::faction::Faction* m_faction;
};

} // faction
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENTTEMPLATE_H


