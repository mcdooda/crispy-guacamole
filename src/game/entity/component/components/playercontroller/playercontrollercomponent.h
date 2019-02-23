#ifndef GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENT_H
#define GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENT_H

#include "playercontrollercomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace playercontroller
{

class PlayerControllerComponent : public ComponentImpl<PlayerControllerComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "playercontroller"; }
		inline static bool enableInMapEditor() { return false; }
		inline static int getUpdatePeriod() { return 1; }

		void update(float time, float dt) override;

	private:
		
};

} // playercontroller
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENT_H



