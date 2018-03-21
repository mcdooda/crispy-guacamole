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

namespace entity
{
class Entity;
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
		void write(const std::vector<entity::Entity*>& entities);

	private:
		void writeHeaders();
		void writeTiles();
		void writeEntities(const std::vector<entity::Entity*>& entities);
		void writeZones();

		template <class T>
		inline void write(T value);

	private:
		const mod::Mod& m_mod;
		const Map& m_map;

		std::ofstream m_file;

		std::map<const flat::video::Texture*, uint16_t> m_tileTextures;
		std::map<const flat::video::Texture*, uint16_t> m_propTextures;
};

template <class T>
inline void Writer::write(T value)
{
	static_assert(std::is_pod<T>::value, "Generic implementation only for pod types");
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <>
inline void Writer::write(const std::string& value)
{
	uint16_t size = static_cast<uint16_t>(value.size());
	write<uint16_t>(size);
	m_file.write(value.data(), size);
}

} // io
} // map
} // game

#endif // GAME_MAP_IO_WRITER_H


