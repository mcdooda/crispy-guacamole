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

void Component::init()
{

}

void Component::deinit()
{

}

void Component::preUpdate(float time, float dt)
{
	FLAT_ASSERT_MSG(false, "This should never be called!");
}

void Component::update(float time, float dt)
{
	FLAT_ASSERT_MSG(false, "This should never be called!");
}

void Component::postUpdate(float time, float dt)
{
	FLAT_ASSERT_MSG(false, "This should never be called!");
}

bool Component::isBusy() const
{
	return false;
}

void Component::cancelCurrentAction()
{

}

#ifdef FLAT_DEBUG
void Component::debugDraw(debug::DebugDisplay& debugDisplay) const
{

}
#endif

ComponentTemplate* Component::loadConfigFile(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath)
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



