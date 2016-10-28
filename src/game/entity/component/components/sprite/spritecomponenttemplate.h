#ifndef GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"
#include "description.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

class SpriteComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const Description& getSpriteDescription() const { return m_spriteDescription; }

	private:
		Description m_spriteDescription;
};

} // sprite
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H


