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

		void init() override;

		void update(float currentTime, float elapsedTime) override;

	private:
		bool addedToMap(Entity* entity, map::Map* map);
		bool removedFromMap(Entity* entity);
};

} // prop
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENT_H

