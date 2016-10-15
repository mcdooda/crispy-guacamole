#include <flat.h>
#include "component.h"
#include "componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{

Component::Component() :
	m_owner(nullptr)
{

}

Component::~Component()
{
	
}

std::shared_ptr<ComponentTemplate> Component::loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath)
{
	FLAT_ASSERT_MSG(false, "Missing specialized method T::loadConfigFile()");
	return nullptr;
}

} // component
} // entity
} // game



