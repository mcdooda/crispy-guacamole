#include "zoneeditormode.h"
#include "../../map/zone.h"

namespace game
{
namespace states
{
namespace editor
{

ZoneEditorMode::ZoneEditorMode(Game& game) : Super(game)
{

}

ZoneEditorMode::~ZoneEditorMode()
{

}

void ZoneEditorMode::setCurrentZone(const std::shared_ptr<map::Zone>& currentZone)
{
	if (map::Zone* previousZone = m_currentZone.lock().get())
	{
		previousZone->eachTileIfExists([previousZone](map::Tile* tile)
		{
			tile->setColor(flat::video::Color::WHITE);
		});
	}

	m_currentZone = currentZone;

	if (map::Zone* currentZone = m_currentZone.lock().get())
	{
		currentZone->eachTileIfExists([currentZone](map::Tile* tile)
		{
			tile->setColor(currentZone->getColor());
		});
	}
}

} // editor
} // states
} // game


