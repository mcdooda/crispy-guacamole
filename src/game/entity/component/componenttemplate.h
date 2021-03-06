#ifndef GAME_ENTITY_COMPONENT_COMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_COMPONENTTEMPLATE_H

#include <flat.h>

namespace game
{
class Game;
namespace entity
{
namespace component
{

class ComponentTemplate : public flat::util::Convertible<ComponentTemplate>
{
	public:
		virtual void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath);
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENTTEMPLATE_H


