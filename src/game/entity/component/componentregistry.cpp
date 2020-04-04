#include "entity/component/componentregistry.h"
#include "entity/component/components/attack/attackcomponent.h"
#include "entity/component/components/behavior/behaviorcomponent.h"
#include "entity/component/components/collision/collisioncomponent.h"
#include "entity/component/components/detection/detectioncomponent.h"
#include "entity/component/components/faction/factioncomponent.h"
#include "entity/component/components/fogvision/fogvisioncomponent.h"
#include "entity/component/components/interaction/interactioncomponent.h"
#include "entity/component/components/life/lifecomponent.h"
#include "entity/component/components/movement/movementcomponent.h"
#include "entity/component/components/playercontroller/playercontrollercomponent.h"
#include "entity/component/components/projectile/projectilecomponent.h"
#include "entity/component/components/prop/propcomponent.h"
#include "entity/component/components/selection/selectioncomponent.h"
#include "entity/component/components/sprite/spritecomponent.h"
#include "entity/component/components/ui/uicomponent.h"
#include "entity/component/components/sample/samplecomponent.h"


namespace game::entity::component
{

ComponentRegistry::ComponentRegistry() :
	m_mapEditorComponentsFilter(0),
	m_entityEditorComponentsFilter(0)
{
	registerComponentTypes();
}

ComponentRegistry::~ComponentRegistry()
{

}

void ComponentRegistry::registerComponentTypes()
{
	// components are updated in the order they are declared
	registerComponentType<playercontroller::PlayerControllerComponent>();
	registerComponentType<detection::DetectionComponent>();
	registerComponentType<behavior::BehaviorComponent>();
	registerComponentType<life::LifeComponent>();
	registerComponentType<attack::AttackComponent>();
	registerComponentType<movement::MovementComponent>();
	registerComponentType<projectile::ProjectileComponent>();
	registerComponentType<collision::CollisionComponent>();
	registerComponentType<sprite::SpriteComponent>();
	registerComponentType<ui::UiComponent>();
	registerComponentType<fogvision::FogVisionComponent>();
	registerComponentType<sample::SampleComponent>();

	// order does not matter for these components (no update method)
	registerComponentType<faction::FactionComponent>();
	registerComponentType<interaction::InteractionComponent>();
	registerComponentType<prop::PropComponent>();
	registerComponentType<selection::SelectionComponent>();
}

const ComponentType& ComponentRegistry::getComponentType(ComponentFlags componentFlag) const
{
	ComponentTypeId componentTypeId = static_cast<ComponentTypeId>(std::log2(componentFlag));
	FLAT_ASSERT(componentTypeId >= 1);
	return *m_componentTypes[componentTypeId - 1].get();
}

} // game::entity::component


