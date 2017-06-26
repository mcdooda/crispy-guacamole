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
		void deinit() override;

		void update(float time, float dt) override;
		
		void enterState(const char* stateName);

		void sleep(float time, float duration);

#ifdef FLAT_DEBUG
		void getThreadDebugInfo(std::string& file, int& line) const;
		void debugDraw(debug::DebugDisplay& debugDisplay) const override;
#endif
		
	private:
		bool addedToMap(Entity* entity, map::Map* map);
		bool entityEnteredVisionRange(Entity* entity);
		bool entityLeftVisionRange(Entity* entity);
		
	private:
		BehaviorRuntime m_behaviorRuntime;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H



