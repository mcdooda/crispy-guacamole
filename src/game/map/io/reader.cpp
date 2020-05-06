#include "map/io/reader.h"
#include "map/io/mapfile.h"
#include "map/map.h"
#include "game.h"
#include "mod/mod.h"

namespace game::map::io
{

Reader::Reader(Game& game, const mod::Mod& mod, Map& map) :
	m_game(game),
	m_mod(mod),
	m_map(map),
	m_reader(game.mapPath + "/map.gpmap")
{

}

Reader::~Reader()
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
	m_map.setState(m_game, m_mod, mapFile);
}

} // game::map::io


