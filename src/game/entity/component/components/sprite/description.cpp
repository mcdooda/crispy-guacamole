#include "description.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

Description::Description() :
	m_atlasWidth(1),
	m_atlasHeight(1),
	m_moveAnimationDescription(nullptr)
{
	
}

void Description::addAnimationDescription(const std::string& animationName, AnimationDescription animationDescription)
{
	FLAT_DEBUG_ONLY(animationDescription.setName(animationName);)
	m_animationDescriptions.emplace(animationName, animationDescription);
}

void Description::cacheMandatoryAnimationDescriptions()
{
	m_moveAnimationDescription = getAnimationDescription("move");
}

const AnimationDescription* Description::getAnimationDescription(const std::string& animationName) const
{
	std::map<std::string, AnimationDescription>::const_iterator it = m_animationDescriptions.find(animationName);
	
	if (it == m_animationDescriptions.cend())
		return nullptr;
		
	return &it->second;
}

} // sprite
} // component
} // entity
} // game



