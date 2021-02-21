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
class EntityUpdater;

namespace component
{
namespace behavior
{

class BehaviorComponent : public ComponentImpl<BehaviorComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "behavior"; }
		inline static const char* getVisualName() { return "Behavior"; }
		inline static bool enableInMapEditor() { return false; }
		inline static bool enableInEntityEditor() { return false; }

		void init() override;
		void deinit() override;

		void update(float time, float dt) override;

		void cancelCurrentAction() override;
		
		void enterState(const char* stateName);
		bool hasState(const char* stateName) const;
		bool setInteractionIfCompatible(const char* stateName, entity::Entity* interactionEntity);
		bool canInteract(const char* stateName) const;

		void sleep(float time, float duration);

		bool handlePlayerMoveOrder(const flat::Vector2& point, Entity* interactionEntity);

		inline EntityHandle getInteractionEntity() const { return m_interactionEntity; }
		inline const std::string& getInteractionStateName() const { return m_interactionStateName; }

#ifdef FLAT_DEBUG
		void getThreadDebugInfo(std::string& file, int& line) const;
		void debugDraw(debug::DebugDisplay& debugDisplay) const override;
#endif
		
	private:
		bool addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);

		bool entityEnteredVisionRange(Entity* entity);
		bool entityLeftVisionRange(Entity* entity);

		bool damageTaken(int amount, Entity* instigator);

		void tryInteracting();
		void cancelInteraction();
		
	private:
		BehaviorRuntime m_behaviorRuntime;
		EntityHandle m_interactionEntity;
		std::string m_interactionStateName;
};

} // behavior
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_BEHAVIOR_BEHAVIORCOMPONENT_H



