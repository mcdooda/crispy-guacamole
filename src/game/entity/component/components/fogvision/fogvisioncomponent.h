#ifndef GAME_ENTITY_COMPONENT_FOGVISION_FOGVISIONCOMPONENT_H
#define GAME_ENTITY_COMPONENT_FOGVISION_FOGVISIONCOMPONENT_H

#include <flat.h>
#include "entity/component/components/fogvision/fogvisioncomponenttemplate.h"
#include "entity/component/component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace fogvision
{

class FogVisionComponent : public ComponentImpl<FogVisionComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "detection"; }

		void update(float currentTime, float elapsedTime) override;
};

} // fogvision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_FOGVISION_FOGVISIONCOMPONENT_H



