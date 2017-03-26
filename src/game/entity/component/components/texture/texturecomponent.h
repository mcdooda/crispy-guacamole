#ifndef GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENT_H
#define GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENT_H

#include <flat.h>
#include "texturecomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
namespace component
{
namespace texture
{

class TextureComponent : public ComponentImpl<TextureComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "texture"; }

		void init() override;

		void update(float currentTime, float elapsedTime) override;

		inline const flat::render::Sprite& getSprite() const { return m_sprite; }

	private:
		bool headingChanged(float heading);
		bool elevationChanged(float elevation);
		bool positionChanged(const flat::Vector3& position);

	private:
		flat::render::Sprite m_sprite;
		bool m_positionChanged : 1;
		bool m_headingChanged : 1;
		bool m_elevationChanged : 1;
};

} // texture
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_TEXTURE_TEXTURECOMPONENT_H



