#ifndef GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"
#include "spritedescription.h"

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

		inline const SpriteDescription& getSpriteDescription() const { return m_spriteDescription; }

	private:
		SpriteDescription m_spriteDescription;
};

} // sprite
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENTTEMPLATE_H


