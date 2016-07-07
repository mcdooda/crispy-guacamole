#include "mapobject.h"

namespace game
{
namespace map
{

MapObject::MapObject()
{
	
}

MapObject::~MapObject()
{
	
}

void MapObject::computeDepth(float x, float y, float radius)
{
	m_depth = x + y + radius;
}

} // map
} // game



