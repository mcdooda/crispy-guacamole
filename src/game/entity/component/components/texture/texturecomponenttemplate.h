#ifndef GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace texture
{

class TextureComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline const std::shared_ptr<const flat::video::Texture>& getTexture() const { return m_texture; }
		
		inline const flat::Vector2& getOrigin() const { return m_origin; }

	private:
		std::shared_ptr<const flat::video::Texture> m_texture;
		flat::Vector2 m_origin;
};

} // texture
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENTTEMPLATE_H


