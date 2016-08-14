#ifndef GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H
#define GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H

#include <queue>
#include <flat.h>
#include "component.h"

namespace game
{
namespace entity
{
namespace component
{

class MovementComponent : public Component
{
	typedef Component Super;
	public:
		void update(float currentTime, float elapsedTime) override;
		
		bool isBusy() const override;
		
		bool followsPath() const;
		void addPointOnPath(const flat::geometry::Vector2& point);
		
	private:
		void separateFromAdjacentTiles();
		
	private:
		std::queue<flat::geometry::Vector2> m_path;
};

} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_MOVEMENTCOMPONENT_H



