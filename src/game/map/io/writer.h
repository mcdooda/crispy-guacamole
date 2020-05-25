#ifndef GAME_MAP_IO_WRITER_H
#define GAME_MAP_IO_WRITER_H

#include <flat.h>

namespace game
{
class Game;

namespace map
{
class Map;

namespace io
{

class Writer
{
	public:
		Writer(Game& game, const Map& map);

		bool canWrite() const;
		void write();

	private:
		Game& m_game;
		const Map& m_map;

		flat::file::serialize::FileWriter m_writer;
};

} // io
} // map
} // game

#endif // GAME_MAP_IO_WRITER_H


