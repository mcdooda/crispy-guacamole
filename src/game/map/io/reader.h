#ifndef GAME_MAP_IO_READER_H
#define GAME_MAP_IO_READER_H

#include <flat.h>

namespace game
{
class Game;

namespace mod
{
class Mod;
}

namespace map
{
class Map;
class TileTemplate;

namespace io
{

class Reader
{
	public:
		Reader(Game& game, Map& map);
		~Reader();
		
		bool canRead() const;
		void read();

	private:
		Game& m_game;
		Map& m_map;
		
		flat::file::serialize::FileReader m_reader;
};

} // io
} // map
} // game

#endif
