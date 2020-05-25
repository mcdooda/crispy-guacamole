#include "writer.h"
#include "game.h"
#include "map/map.h"

namespace game
{
namespace map
{
namespace io
{

Writer::Writer(Game& game, const Map& map) :
	m_game(game),
	m_map(map),
	m_writer(game.mod.getMapFilePath(game.mapName, "map.gpmap").string())
{

}

bool Writer::canWrite() const
{
	return m_writer.canWrite();
}

void Writer::write()
{
	MapFile mapFile;
	m_map.getState(m_game, mapFile);
	m_writer.Processor::process(mapFile);
}

} // io
} // map
} // game


