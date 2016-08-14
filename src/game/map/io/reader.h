#ifndef GAME_MAP_IO_READER_H
#define GAME_MAP_IO_READER_H

#include <vector>
#include <fstream>
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

namespace io
{

class Reader
{
	public:
		Reader(Game* game, const mod::Mod& mod, Map& map);
		~Reader();
		
		void read();

	private:
		void readHeaders();
		void readTiles();
		void readEntities();

		bool readBool();
		float readFloat();
		uint16_t readUint16();
		std::string readString();
		
		Game* m_game;
		const mod::Mod& m_mod;
		Map& m_map;
		
		std::ifstream m_file;

		int m_mapWidth;
		int m_mapHeight;
		
		std::vector<std::shared_ptr<const flat::video::FileTexture>> m_tileTextures;
		std::vector<std::shared_ptr<const flat::video::FileTexture>> m_propTextures;
};

} // io
} // map
} // game

#endif
