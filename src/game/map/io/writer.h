#ifndef GAME_MAP_IO_WRITER_H
#define GAME_MAP_IO_WRITER_H

#include <cstdint>
#include <string>
#include <fstream>

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

class Writer
{
	public:
		Writer(const mod::Mod& mod, const std::string& mapName, const Map& map);
		~Writer();

		bool canWrite() const;
		void write();

	private:
		void writeHeaders();
		void writeTiles();
		void writeEntities();

		void writeBool(bool value);
		void writeFloat(float value);
		void writeInt16(int16_t value);
		void writeUint16(uint16_t value);
		void writeString(const std::string& value);

	private:
		const mod::Mod& m_mod;
		const Map& m_map;

		std::ofstream m_file;

		std::map<const flat::video::Texture*, uint16_t> m_tileTextures;
		std::map<const flat::video::Texture*, uint16_t> m_propTextures;
};

} // io
} // map
} // game

#endif // GAME_MAP_IO_WRITER_H


