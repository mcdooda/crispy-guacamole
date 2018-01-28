#ifndef GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENT_H

#include <flat.h>
#include <set>
#include "detectioncomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace detection
{

class DetectionComponent : public ComponentImpl<DetectionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "detection"; }

		void init(lua_State* L) override;
		void deinit(lua_State* L) override;

		void update(lua_State* L, float currentTime, float elapsedTime) override;

		bool isVisible(const Entity& target) const;

		inline const std::set<Entity*>& getVisibleEntities() const { return m_visibleEntities; }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool removedFromMap(lua_State* L, Entity* entity);
		bool visibleEntityRemovedFromMap(lua_State* L, Entity* entity);

	public:
		flat::Slot<lua_State*, Entity*> entityEnteredVisionRange;
		flat::Slot<lua_State*, Entity*> entityLeftVisionRange;

	private:
		std::set<Entity*> m_visibleEntities;
};

} // detection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENT_H



