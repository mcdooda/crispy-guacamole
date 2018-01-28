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
		inline static bool enableInMapEditor() { return false; }
		inline static bool enableInEntityEditor() { return false; }

		void init(lua_State* L) override;
		void deinit(lua_State* L) override;

		void update(lua_State* L, float time, float dt) override;
		
		void enterState(lua_State* L, const char* stateName);

		void sleep(float time, float duration);

#ifdef FLAT_DEBUG
		void getThreadDebugInfo(std::string& file, int& line) const;
		void debugDraw(debug::DebugDisplay& debugDisplay) const override;
#endif
		
	private:
		bool addedToMap(lua_State* L, Entity* entity, map::Map* map);
		bool entityEnteredVisionRange(lua_State* L, Entity* entity);
		bool entityLeftVisionRange(lua_State* L, Entity* entity);
		
	private:
		BehaviorRuntime m_behaviorRuntime;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H



