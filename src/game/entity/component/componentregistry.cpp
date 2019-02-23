#include "componentregistry.h"
#include "components/attack/attackcomponent.h"
#include "components/behavior/behaviorcomponent.h"
#include "components/collision/collisioncomponent.h"
#include "components/detection/detectioncomponent.h"
#include "components/faction/factioncomponent.h"
#include "components/interaction/interactioncomponent.h"
#include "components/life/lifecomponent.h"
#include "components/movement/movementcomponent.h"
#include "components/playercontroller/playercontrollercomponent.h"
#include "components/projectile/projectilecomponent.h"
#include "components/prop/propcomponent.h"
#include "components/selection/selectioncomponent.h"
#include "components/sprite/spritecomponent.h"
#include "components/ui/uicomponent.h"

namespace game
{
namespace entity
{
namespace component
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

	// order does not matter for these components
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

} // component
} // entity
} // game


