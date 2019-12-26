#include "entity/component/components/fogvision/fogvisioncomponent.h"
#include "entity/component/components/detection/detectioncomponent.h"
#include "entity/entity.h"
#include "map/map.h"

namespace game
{
namespace entity
{
namespace component
{
namespace fogvision
{

void FogVisionComponent::update(float currentTime, float elapsedTime)
{
	const detection::DetectionComponentTemplate* detectionComponentTemplate = getTemplate<detection::DetectionComponent>();
	if (detectionComponentTemplate == nullptr)
	{
		return;
	}

	map::fog::Fog& fog = m_owner->getMap()->getFog();
	const float visionRange = detectionComponentTemplate->getVisionRange();
	fog.discover(m_owner->getPosition2d(), visionRange);
}

} // fogvision
} // component
} // entity
} // game


