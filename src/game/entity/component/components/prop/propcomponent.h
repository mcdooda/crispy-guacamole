#ifndef GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H
#define GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H

#include <flat.h>
#include "propcomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

class PropComponent : public ComponentImpl<PropComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "prop"; }
		inline static const char* getVisualName() { return "Prop"; }
		inline static bool requiresUpdate() { return false; }

		void init() override;
		void deinit() override;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool addedToMap(Entity* entity, map::Map* map);
		bool removedFromMap(Entity* entity);
};

} // prop
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H


