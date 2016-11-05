#include "componentregistry.h"
#include "components/behavior/behaviorcomponent.h"
#include "components/detection/detectioncomponent.h"
#include "components/collision/collisioncomponent.h"
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

ComponentRegistry::ComponentRegistry()
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
	registerComponentType<movement::MovementComponent>();
	registerComponentType<projectile::ProjectileComponent>();
	registerComponentType<collision::CollisionComponent>();
	registerComponentType<sprite::SpriteComponent>();
	registerComponentType<texture::TextureComponent>();
}

} // component
} // entity
} // game


