#include "displaymanager.h"
#include "mapobject.h"

namespace game
{
namespace map
{

DisplayManager::DisplayManager()
{
	
}

DisplayManager::~DisplayManager()
{
	
}

void DisplayManager::clearAll()
{
	m_objects.clear();
}

void DisplayManager::add(const MapObject* mapObject)
{
	m_objects.push_back(mapObject);
}

void DisplayManager::sortByDepthAndDraw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix)
{
	std::sort(std::begin(m_objects), std::end(m_objects), &DisplayManager::isLessDeep);
	for (const MapObject* mapObject : m_objects)
	{
		mapObject->draw(renderSettings, viewMatrix);
	}
}

bool DisplayManager::isLessDeep(const MapObject* a, const MapObject* b)
{
	return a->getDepth() < b->getDepth();
}

} // map
} // game



