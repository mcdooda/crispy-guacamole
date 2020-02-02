#ifndef GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H

#include "entity/component/componenttemplate.h"
#include "map/navigability.h"

namespace game::entity::component::prop
{

class PropComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::string& entityTemplatePath) override final;

		inline int getWidth() const { return m_size.x; }
		inline int getHeight() const { return m_size.y; }
		inline map::Navigability getNavigability() const { return m_navigability; }

	private:
		flat::Vector2i m_size;
		map::Navigability m_navigability;
};

} // game::entity::component::prop

#endif // GAME_ENTITY_COMPONENT_PROP_PROPCOMPONENTTEMPLATE_H


