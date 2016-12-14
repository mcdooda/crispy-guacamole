#include "componentregistry.h"
#include "components/attack/attackcomponent.h"
#include "components/behavior/behaviorcomponent.h"
#include "components/collision/collisioncomponent.h"
#include "components/detection/detectioncomponent.h"
#include "components/faction/factioncomponent.h"
#include "components/movement/movementcomponent.h"
#include "components/projectile/projectilecomponent.h"
#include "components/sprite/spritecomponent.h"
#include "components/texture/texturecomponent.h"

namespace game
{
namespace entity
{
namespace component
{

ComponentRegistry::ComponentRegistry() :
	m_editorComponentsFilter(0)
{
	registerComponentTypes();
}

ComponentRegistry::~ComponentRegistry()
{

}

void ComponentRegistry::registerComponentTypes()
{
	// components are updated in the order they are declared
	registerComponentType<detection::DetectionComponent>();
	registerComponentType<behavior::BehaviorComponent>();
	registerComponentType<attack::AttackComponent>();
	registerComponentType<movement::MovementComponent>();
	registerComponentType<projectile::ProjectileComponent>();
	registerComponentType<collision::CollisionComponent>();
	registerComponentType<sprite::SpriteComponent>();
	registerComponentType<texture::TextureComponent>();

	// order does not matter for these components
	registerComponentType<faction::FactionComponent>();
}

} // component
} // entity
} // game


