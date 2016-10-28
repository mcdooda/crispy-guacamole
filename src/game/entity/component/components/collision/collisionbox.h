#ifndef GAME_ENTITY_COMPONENT_COLLISION_COLLISIONBOX_H
#define GAME_ENTITY_COMPONENT_COLLISION_COLLISIONBOX_H

#include <flat.h>

namespace game
{
namespace entity
{
namespace component
{

enum class CollisionType : uint8_t
{
	SPHERE,
	CYLINDER
};

struct Sphere
{
	float radius;
};

struct Cylinder
{
	float radius;
	float height;
};

struct CollisionBox
{
	CollisionType type;
	union
	{
		Sphere sphere;
		Cylinder cylinder;
	};

	inline float getRadius() const
	{
		switch (type)
		{
		case CollisionType::SPHERE:
			return sphere.radius;
		case CollisionType::CYLINDER:
			return cylinder.radius;
		}
		FLAT_ASSERT(false);
		return 0.f;
	}

	static bool collides(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration);

private:
	static bool sphereVsSphere(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration);
	static bool sphereVsCylinder(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration);
	static bool cylinderVsCylinder(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration);
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISION_COLLISIONBOX_H


