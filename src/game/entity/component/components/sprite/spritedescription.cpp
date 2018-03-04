#include "spritedescription.h"

namespace game
{
namespace entity
{
namespace component
{
namespace sprite
{

SpriteDescription::SpriteDescription() :
	m_atlasWidth(1),
	m_atlasHeight(1)
{
	
}

void SpriteDescription::addAnimationDescription(const std::string& animationName, AnimationDescription animationDescription)
{
	FLAT_DEBUG_ONLY(animationDescription.setName(animationName);)
	m_animationDescriptions.emplace(animationName, animationDescription);
}

const AnimationDescription* SpriteDescription::getAnimationDescription(const std::string& animationName) const
{
	std::map<std::string, AnimationDescription>::const_iterator it = m_animationDescriptions.find(animationName);
	
	if (it == m_animationDescriptions.cend())
		return nullptr;
		
	return &it->second;
}

void SpriteDescription::addAttachPoint(const std::string& attachPointName, const flat::Vector2& attachPoint)
{
	m_attachPoints.emplace(attachPointName, attachPoint);
}

const flat::Vector2* SpriteDescription::getAttachPoint(const std::string& attachPointName) const
{
	std::map<std::string, flat::Vector2>::const_iterator it = m_attachPoints.find(attachPointName);

	if (it == m_attachPoints.cend())
		return nullptr;

	return &it->second;
}

} // sprite
} // component
} // entity
} // game



