#ifndef GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENT_H
#define GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENT_H

#include "component.h"
#include "../behavior/behaviorruntime.h"

namespace game
{
namespace map
{
class Map;
}
namespace entity
{
namespace component
{

class BehaviorComponent : public Component
{
	typedef Component Super;
	public:
		void setOwner(Entity* entity) override;
		void update(float currentTime, float elapsedTime) override;
		
		void enterState(const char* stateName);
		
	private:
		void addedToMap(map::Map* map);
		
	private:
		behavior::BehaviorRuntime m_behaviorRuntime;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIORCOMPONENT_H



