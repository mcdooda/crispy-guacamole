#ifndef GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace prop
{

class PropComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

	private:
		int m_width;
		int m_height;
};

} // prop
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H


