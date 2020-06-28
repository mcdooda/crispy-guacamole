#ifndef GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H
#define GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H

#include <flat.h>

#include "propcomponenttemplate.h"
#include "entity/component/component.h"

namespace game
{
namespace entity
{
class EntityUpdater;

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

		void selectAnimation(
			const EntityTemplate* topLeftTemplate,
			const EntityTemplate* topRightTemplate,
			const EntityTemplate* bottomLeftTemplate,
			const EntityTemplate* bottomRightTemplate);

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);
		bool removedFromMap(Entity* entity);

		void selectAnimationPostCall();
};

} // prop
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H


