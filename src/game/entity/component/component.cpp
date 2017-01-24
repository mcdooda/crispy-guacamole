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
	, m_debug(true)
#endif
{

}

Component::~Component()
{
	
}

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



