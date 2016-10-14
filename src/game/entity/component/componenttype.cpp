#include "componenttype.h"

namespace game
{
namespace entity
{
namespace component
{

ComponentFlags ComponentType::getComponentTypeFlag() const
{
	ComponentFlags flags = 1 << getComponentTypeId();
	FLAT_ASSERT(flags != 0);
	return flags;
}

} // component
} // entity
} // game


