#ifndef GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"
#include "../../../sprite/description.h"

namespace game
{
namespace entity
{
namespace component
{

class SpriteComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const sprite::Description& getSpriteDescription() const { return m_spriteDescription; }

	private:
		sprite::Description m_spriteDescription;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITECOMPONENTTEMPLATE_H


