#ifndef GAME_MAP_DISPLAYMANAGER
#define GAME_MAP_DISPLAYMANAGER

#include <vector>
#include <flat.h>

namespace game
{
namespace map
{
class MapObject;

class DisplayManager final
{
	public:
		DisplayManager() {}
		~DisplayManager() {}
		
		inline void clearAll() { m_objects.clear(); }
		inline void add(const MapObject* mapObject) { m_objects.push_back(mapObject); }
		void sortByDepthAndDraw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix);
		
	private:
		std::vector<const MapObject*> m_objects;
};

} // map
} // game

#endif // GAME_MAP_DISPLAYMANAGER



