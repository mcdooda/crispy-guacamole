#include "map/io/reader.h"
#include "map/io/mapfile.h"
#include "map/map.h"
#include "game.h"
#include "mod/mod.h"

namespace game::map::io
{

Reader::Reader(Game& game, Map& map) :
	m_game(game),
	m_map(map),
	m_reader(game.mapPath / "map.gpmap")
{

}

bool Reader::canRead() const
{
	return m_reader.canRead();
}

void Reader::read()
{
	MapFile mapFile;
	m_reader.Processor::process(mapFile);
	m_map.setState(m_game, mapFile);
}

} // game::map::io


