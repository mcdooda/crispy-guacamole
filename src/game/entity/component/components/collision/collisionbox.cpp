#include "collisionbox.h"

namespace game
{
namespace entity
{
namespace component
{

bool CollisionBox::collides(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration)
{
	if (a.type == CollisionType::SPHERE && b.type == CollisionType::SPHERE)
	{
		return sphereVsSphere(aPos, bPos, a, b, penetration);
	}
	else if (a.type == CollisionType::CYLINDER && b.type == CollisionType::CYLINDER)
	{
		return cylinderVsCylinder(aPos, bPos, a, b, penetration);
	}
	else if (a.type == CollisionType::SPHERE && b.type == CollisionType::CYLINDER)
	{
		return sphereVsCylinder(aPos, bPos, a, b, penetration);
	}
	else if (a.type == CollisionType::CYLINDER && b.type == CollisionType::SPHERE)
	{
		const bool collides = sphereVsCylinder(bPos, aPos, b, a, penetration);
		penetration = -penetration;
		return collides;
	}
	FLAT_ASSERT(false);
	return false;
}

bool CollisionBox::sphereVsSphere(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration)
{
	FLAT_ASSERT(a.type == CollisionType::SPHERE && b.type == CollisionType::SPHERE);
	const float radiusSum2 = (a.sphere.radius + b.sphere.radius) * (a.sphere.radius + b.sphere.radius);
	const float distance2 = flat::length2(aPos - bPos);
	if (distance2 < radiusSum2)
	{
		penetration = flat::normalize(bPos - aPos);
		penetration *= a.sphere.radius + b.sphere.radius - std::sqrt(distance2);
		return true;
	}
	return false;
}

bool CollisionBox::sphereVsCylinder(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration)
{
	// wrong for top and bottom cases: the cylinder is considered as a capsule here
	FLAT_ASSERT(a.type == CollisionType::SPHERE && b.type == CollisionType::CYLINDER);
	const float bottom = bPos.z;
	const float top = bottom + b.cylinder.height;
	const float radiusSum2 = (a.sphere.radius + b.cylinder.radius) * (a.sphere.radius + b.cylinder.radius);
	if (bottom <= aPos.z && aPos.z <= top)
	{
		const float distance2 = (bPos.x - aPos.x) * (bPos.x - aPos.x) + (bPos.y - aPos.y) * (bPos.y - aPos.y);
		if (distance2 < radiusSum2)
		{
			penetration = flat::normalize(bPos - aPos);
			penetration *= a.sphere.radius + b.cylinder.radius - std::sqrt(distance2);
			penetration.z = 0.f;
			return true;
		}
		return false;
	}
	else if (aPos.z < bottom)
	{
		if (flat::length2(aPos - bPos) < radiusSum2)
		{
			penetration = flat::normalize(bPos - aPos);
			penetration *= a.sphere.radius + b.cylinder.radius - flat::length2(bPos - aPos);
			return true;
		}
		return false;
	}
	else if (aPos.z > top)
	{
		flat::Vector3 cylinderTopPos = bPos;
		cylinderTopPos.z = top;
		if (flat::length2(aPos - cylinderTopPos) < radiusSum2)
		{
			penetration = flat::normalize(bPos - cylinderTopPos);
			penetration *= a.sphere.radius + b.cylinder.radius - flat::length2(bPos - cylinderTopPos);
			return true;
		}
		return false;
	}
	FLAT_ASSERT(false);
	return false;
}

bool CollisionBox::cylinderVsCylinder(const flat::Vector3& aPos, const flat::Vector3& bPos, const CollisionBox& a, const CollisionBox& b, flat::Vector3& penetration)
{
	FLAT_ASSERT(a.type == CollisionType::CYLINDER && b.type == CollisionType::CYLINDER);
	const float radiusSum2 = (a.cylinder.radius + b.cylinder.radius) * (a.cylinder.radius + b.cylinder.radius);
	if (flat::length2(aPos - bPos) < radiusSum2)
	{
		const float aBottom = aPos.z;
		const float aTop = aBottom + a.cylinder.height;
		const float bBottom = bPos.z;
		const float bTop = bBottom + b.cylinder.height;
		if ((aBottom <= bBottom && bBottom <= aTop) || (aBottom <= bTop && bTop <= aTop)
		 || (bBottom <= aBottom && aBottom <= bTop) || (bBottom <= aTop && aTop <= bTop))
		{
			flat::Vector3 direction = bPos - aPos;
			direction.z = 0.f;
			penetration = flat::normalize(direction);
			penetration *= a.cylinder.radius + b.cylinder.radius - flat::length(direction);
			return true;
		}
	}
	return false;
}

} // component
} // entity
} // game


