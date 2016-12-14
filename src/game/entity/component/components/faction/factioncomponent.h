#ifndef GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENT_H

#include "factioncomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace faction
{
class Faction;
}
namespace component
{
namespace faction
{

class FactionComponent : public ComponentImpl<FactionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "faction"; }

		void update(float currentTime, float elapsedTime) override;

		const entity::faction::Faction& getFaction() const;
};

} // faction
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENT_H



