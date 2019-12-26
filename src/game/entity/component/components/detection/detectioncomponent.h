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
		//inline static int getUpdatePeriod() { return 20; } // TODO: update self/nearby when moving 

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		bool isVisible(const Entity& target) const;

		inline const std::set<Entity*>& getVisibleEntities() const { return m_visibleEntities; }

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool removedFromMap(Entity* entity);
		bool visibleEntityRemovedFromMap(Entity* entity);

		void discoverFog();

	public:
		flat::Slot<Entity*> entityEnteredVisionRange;
		flat::Slot<Entity*> entityLeftVisionRange;

	private:
		std::set<Entity*> m_visibleEntities;
};

} // detection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENT_H



