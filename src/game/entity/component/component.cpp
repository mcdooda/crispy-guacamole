#include <flat.h>
#include "component.h"
#include "componenttemplate.h"
#include "../entity.h"

namespace game
{
namespace entity
{
namespace component
{

Component::Component() :
	m_owner(nullptr),
	m_disableLevel(0)
#ifdef FLAT_DEBUG
	, m_debug(false)
#endif
{

}

Component::~Component()
{
	
}

void Component::init(lua_State* L)
{

}

void Component::deinit(lua_State* L)
{

}

void Component::update(lua_State* L, float time, float dt)
{

}

bool Component::isBusy() const
{
	return false;
}

#ifdef FLAT_DEBUG
void Component::debugDraw(debug::DebugDisplay & debugDisplay) const
{

}
#endif

ComponentTemplate* Component::loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_ASSERT_MSG(false, "Missing specialized method T::loadConfigFile()");
	return nullptr;
}

const EntityTemplate& Component::getEntityTemplate() const
{
	return *m_owner->getEntityTemplate().get();
}

} // component
} // entity
} // game



