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
	map::Map* map = m_owner->getMap();
	if (!map->isLoaded())
	{
		return;
	}

	const detection::DetectionComponentTemplate* detectionComponentTemplate = getTemplate<detection::DetectionComponent>();
	if (detectionComponentTemplate == nullptr)
	{
		return;
	}

	const float visionRange = detectionComponentTemplate->getVisionRange();
	map::fog::Fog& fog = map->getFog();
	fog.discover(m_owner->getPosition2d(), visionRange);
}

} // fogvision
} // component
} // entity
} // game


