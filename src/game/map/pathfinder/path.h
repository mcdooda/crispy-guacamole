#ifndef GAME_MAP_PATHFINDER_PATH_H
#define GAME_MAP_PATHFINDER_PATH_H

#include <flat.h>
#include "map/navigability.h"

namespace game::map
{
class Map;
} // game::map

namespace game::map::pathfinder
{

class Path
{
    friend class Pathfinder;
    public:
        Path() = default;
        Path(Path&& path) = default;
        Path(Path& path) = default;

#ifdef FLAT_DEBUG
        static void enableSimplifyPath(bool shouldSimplifyPath) { Path::shouldSimplifyPath = shouldSimplifyPath; }
#endif

        void simplify(const Map& map, float jumpHeight, Navigability navigabilityMask);

        inline const std::vector<flat::Vector2>& getPoints() const { return m_positions; }
        std::vector<flat::Vector2> getUniqueTilePositions() const;
        inline const flat::Vector2& getPoint(int index) const { return m_positions[index]; }
        inline int getPointsCount() const { return static_cast<int>(m_positions.size()); }
        inline const flat::Vector2& getLastPoint() const { return m_positions.back(); }
        inline const flat::Vector2& getFirstPoint() const { return m_positions.front(); }
        inline bool isEmpty() const { return m_positions.empty(); }

        inline void clear() { return m_positions.clear(); }

        void insertPath(const Path& path);
        void insertPoint(const flat::Vector2& point);

    private:
#ifdef FLAT_DEBUG
        static bool shouldSimplifyPath;
#endif
        std::vector<flat::Vector2> m_positions;
};

} // game::map::pathfinder
#endif // GAME_MAP_PATHFINDER_PATH_H
