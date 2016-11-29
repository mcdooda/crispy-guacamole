#ifndef GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H
#define GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H

#include "behaviorcomponenttemplate.h"
#include "behaviorruntime.h"
#include "../../component.h"

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
namespace behavior
{

class BehaviorComponent : public ComponentImpl<BehaviorComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "behavior"; }
		inline static bool enableInEditor() { return false; }

		void init() override;

		void update(float currentTime, float elapsedTime) override;
		
		void enterState(const char* stateName);

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)
		
	private:
		void addedToMap(map::Map* map);
		void entityEnteredVisionRange(entity::Entity* entity);
		void entityLeftVisionRange(entity::Entity* entity);
		
	private:
		BehaviorRuntime m_behaviorRuntime;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H



