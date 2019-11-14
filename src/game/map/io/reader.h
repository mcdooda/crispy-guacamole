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
class TileTemplate;

namespace io
{

class Reader
{
	public:
		Reader(Game& game, const mod::Mod& mod, Map& map);
		~Reader();
		
		bool canRead() const;
		void read();

	private:
		void readHeaders();
		void readTiles();
		void readEntities();
		void readZones();

		template <class T>
		inline void read(T& value);
		
	private:
		Game& m_game;
		const mod::Mod& m_mod;
		Map& m_map;
		
		std::ifstream m_file;

		int m_minX;
		int m_maxX;
		int m_minY;
		int m_maxY;
		
		std::vector<std::shared_ptr<const TileTemplate>> m_tileTemplates;
		std::vector<std::shared_ptr<const flat::video::FileTexture>> m_propTextures;
};

template <class T>
inline void Reader::read(T& value)
{
	static_assert(std::is_pod<T>::value, "Generic implementation only for pod types");
	m_file.read(reinterpret_cast<char*>(&value), sizeof(T));
}

template <>
inline void Reader::read(std::string& value)
{
	uint16_t size;
	read<uint16_t>(size);
	value.resize(size);
	m_file.read(&value[0], size);
}

template <>
inline void Reader::read(flat::Vector2& value)
{
	read<float>(value.x);
	read<float>(value.y);
}

} // io
} // map
} // game

#endif
