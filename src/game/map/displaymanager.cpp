#include "displaymanager.h"
#include "mapobject.h"

namespace game
{
namespace map
{

inline bool isLessDeep(const MapObject* a, const MapObject* b)
{
	const float aDepth = a->getDepth();
	const float bDepth = b->getDepth();
	if (aDepth == bDepth)
	{
		float ipart;
		const float fpart = std::modf(aDepth, &ipart);
		if (fpart < 0.5f)
		{
			return a->getTextureHash() < b->getTextureHash();
		}
		else
		{
			return a->getTextureHash() > b->getTextureHash();
		}
	}
	else
	{
		return aDepth < bDepth;
	}
}

void DisplayManager::sortByDepthAndDraw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix)
{
	std::sort(std::begin(m_objects), std::end(m_objects), &isLessDeep);

	/*for (const MapObject* mapObject : m_objects)
	{
		mapObject->getSprite().draw(renderSettings, viewMatrix);
	}*/

	flat::util::SpriteBatch spriteBatch;
	std::vector<const MapObject*>::iterator it = m_objects.begin();
	std::vector<const MapObject*>::iterator end = m_objects.end();
	while (it != m_objects.end())
	{
		spriteBatch.clear();

		const MapObject* mapObject = *it;
		std::vector<const MapObject*>::iterator it2 = std::upper_bound(it, end, mapObject, &isLessDeep);
		while (it != it2)
		{
			mapObject = *it;
			spriteBatch.add(mapObject->getSprite());
			++it;
		}

		spriteBatch.draw(renderSettings, viewMatrix);
	}
}

} // map
} // game



