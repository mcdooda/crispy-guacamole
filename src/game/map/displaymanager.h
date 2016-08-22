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
		DisplayManager();
		~DisplayManager();
		
		void clearAll();
		void add(const MapObject* mapObject);
		void sortByDepthAndDraw(const flat::util::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix);
		
	private:
		static bool isLessDeep(const MapObject* a, const MapObject* b);
		
		std::vector<const MapObject*> m_objects;
};

} // map
} // game

#endif // GAME_MAP_DISPLAYMANAGER



