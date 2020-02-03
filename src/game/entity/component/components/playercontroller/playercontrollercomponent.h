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
		inline static const char* getVisualName() { return "Player Controller"; }
		inline static bool enableInMapEditor() { return false; }
		inline static int getUpdatePeriod() { return 1; }

		void init() override;

		void update(float time, float dt) override;

		inline void setGamepadIndex(flat::input::GamepadIndex gamepadIndex) { m_gamepadIndex = gamepadIndex; }
		inline flat::input::GamepadIndex getGamepadIndex() const { return m_gamepadIndex; }

	private:
		flat::input::GamepadIndex m_gamepadIndex;
};

} // playercontroller
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PLAYERCONTROLLER_PLAYERCONTROLLERCOMPONENT_H



