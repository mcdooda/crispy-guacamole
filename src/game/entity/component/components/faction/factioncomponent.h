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
		inline static const char* getVisualName() { return "Faction"; }
		inline static ComponentUpdateType getUpdateType() { return ComponentUpdateType::NONE; }

		const entity::faction::Faction& getFaction() const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)
};

} // faction
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_FACTION_FACTIONCOMPONENT_H



