#ifndef GAME_MAP_GAMEMAP_H
#define GAME_MAP_GAMEMAP_H

#include "map.h"

namespace game
{
namespace map
{

class GameMap : public Map
{
	public:
		GameMap();
		~GameMap() override;

		Tile* getTile(int x, int y) override;

		void eachTile(std::function<void(Tile*)> func) override;

	protected:
		void createTiles() override;
		void destroyTiles();

		int getTileIndex(int x, int y) const;
		int getWidth() const;
		int getHeight() const;
		int getNumTiles() const;

	private:
		Tile* m_tiles;
};

} // map
} // game

#endif // GAME_MAP_GAMEMAP_H


