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
		Reader(Game& game, const mod::Mod& mod, const std::string& mapName, Map& map);
		~Reader();
		
		bool canRead() const;
		void read(lua_State* L);

	private:
		void readConfig(lua_State* L);
		void readHeaders();
		void readTiles();
		void readEntities();

		template <class T>
		void read(T& value);

		template <>
		void read(std::string& value);
		
	private:
		Game& m_game;
		const mod::Mod& m_mod;
		std::string m_mapName;
		Map& m_map;
		
		std::ifstream m_file;

		int m_minX;
		int m_maxX;
		int m_minY;
		int m_maxY;
		
		std::vector<std::shared_ptr<const flat::video::FileTexture>> m_tileTextures;
		std::vector<std::shared_ptr<const flat::video::FileTexture>> m_propTextures;
};

template <class T>
void Reader::read(T& value)
{
	m_file.read(reinterpret_cast<char*>(&value), sizeof(T));
}

template <>
void Reader::read(std::string& value)
{
	uint16_t size;
	read<uint16_t>(size);
	value.resize(size);
	m_file.read(&value[0], size);
}

} // io
} // map
} // game

#endif
