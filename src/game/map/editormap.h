#ifndef GAME_MAP_EDITORMAP_H
#define GAME_MAP_EDITORMAP_H

#include "map.h"

namespace game
{
namespace map
{

class EditorMap : public Map
{
public:
	EditorMap();
	~EditorMap() override;

	Tile* getTile(int x, int y) override;

	void eachTile(std::function<void(Tile*)> func) override;

protected:
	void createTiles() override;

private:
	using TilePosition = std::pair<int, int>;
	std::map<TilePosition, Tile> m_tiles;
};

} // map
} // game

#endif // GAME_MAP_EDITORMAP_H


