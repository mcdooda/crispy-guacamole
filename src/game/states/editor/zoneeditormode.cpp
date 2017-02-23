#include "zoneeditormode.h"
#include "../../map/map.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

ZoneEditorMode::ZoneEditorMode(Game& game) : Super(game),
	m_drawingRectangle(false),
	m_selectedRectangle(nullptr)
{

}

ZoneEditorMode::~ZoneEditorMode()
{

}

void ZoneEditorMode::clearBrush() const
{
	map::Zone* previousZone = m_currentZone.lock().get();

	if (previousZone != nullptr)
	{
		previousZone->eachTileIfExists([previousZone](map::Tile* tile)
		{
			tile->setColor(flat::video::Color::WHITE);
		});
	}

	if (m_drawingRectangle
		&& (m_currentRectangle.minX < m_currentRectangle.maxX || m_currentRectangle.minY < m_currentRectangle.maxY))
	{
		map::Map& map = getMap();
		for (int x = m_currentRectangle.minX; x <= m_currentRectangle.maxX; ++x)
		{
			for (int y = m_currentRectangle.minY; y <= m_currentRectangle.maxY; ++y)
			{
				if (map::Tile* tile = map.getTileIfExists(x, y))
				{
					tile->setColor(flat::video::Color::WHITE);
				}
			}
		}
	}

	if (m_selectedRectangle != nullptr)
	{
		map::Map& map = getMap();
		for (int x = m_selectedRectangle->minX; x <= m_selectedRectangle->maxX; ++x)
		{
			for (int y = m_selectedRectangle->minY; y <= m_selectedRectangle->maxY; ++y)
			{
				if (map::Tile* tile = map.getTileIfExists(x, y))
				{
					tile->setColor(flat::video::Color::WHITE);
				}
			}
		}
	}
}

void ZoneEditorMode::displayBrush() const
{
	map::Zone* currentZone = m_currentZone.lock().get();
	if (currentZone != nullptr)
	{
		currentZone->eachTileIfExists([currentZone](map::Tile* tile)
		{
			tile->setColor(currentZone->getColor());
		});
	}

	if (m_drawingRectangle
		&& (m_currentRectangle.minX < m_currentRectangle.maxX || m_currentRectangle.minY < m_currentRectangle.maxY))
	{
		map::Map& map = getMap();
		for (int x = m_currentRectangle.minX; x <= m_currentRectangle.maxX; ++x)
		{
			for (int y = m_currentRectangle.minY; y <= m_currentRectangle.maxY; ++y)
			{
				if (map::Tile* tile = map.getTileIfExists(x, y))
				{
					tile->setColor(currentZone->getColor());
				}
			}
		}
	}

	if (m_selectedRectangle != nullptr)
	{
		map::Map& map = getMap();
		for (int x = m_selectedRectangle->minX; x <= m_selectedRectangle->maxX; ++x)
		{
			for (int y = m_selectedRectangle->minY; y <= m_selectedRectangle->maxY; ++y)
			{
				if (map::Tile* tile = map.getTileIfExists(x, y))
				{
					tile->setColor(flat::video::Color::RED);
				}
			}
		}
	}
}

void ZoneEditorMode::updateBrushTiles()
{
	if (m_currentZone.expired())
	{
		return;
	}

	flat::input::Mouse* mouse = m_game.input->mouse;

	if (mouse->isJustPressed(M(LEFT)))
	{
		m_mouseDownMapPosition = m_brushPosition;
		m_drawingRectangle = true;
		m_selectedRectangle = nullptr;
	}
	
	if (m_drawingRectangle)
	{
		m_currentRectangle.minX = static_cast<int>(std::round(m_mouseDownMapPosition.x));
		m_currentRectangle.minY = static_cast<int>(std::round(m_mouseDownMapPosition.y));
		m_currentRectangle.maxX = static_cast<int>(std::round(m_brushPosition.x));
		m_currentRectangle.maxY = static_cast<int>(std::round(m_brushPosition.y));

		if (m_currentRectangle.minX > m_currentRectangle.maxX)
		{
			std::swap(m_currentRectangle.minX, m_currentRectangle.maxX);
		}
		if (m_currentRectangle.minY > m_currentRectangle.maxY)
		{
			std::swap(m_currentRectangle.minY, m_currentRectangle.maxY);
		}

		if (mouse->isJustReleased(M(LEFT)))
		{
			map::Zone* currentZone = m_currentZone.lock().get();
			FLAT_ASSERT(currentZone != nullptr);

			if (flat::length2(m_mouseDownMapPosition - m_brushPosition) > 0.5f)
			{
				// add new rectangle
				m_selectedRectangle = &currentZone->addRectangle(m_currentRectangle);
			}
			else
			{
				// select existing rectangle
				m_selectedRectangle = currentZone->selectRectangle(m_mouseDownMapPosition);
			}
			m_drawingRectangle = false;
		}
	}
	
}

void ZoneEditorMode::handleShortcuts()
{
	const flat::input::Keyboard* keyboard = m_game.input->keyboard;

	if (keyboard->isJustPressed(K(DELETE)))
	{
		if (m_selectedRectangle != nullptr)
		{
			map::Map& map = getMap();
			for (int x = m_selectedRectangle->minX; x <= m_selectedRectangle->maxX; ++x)
			{
				for (int y = m_selectedRectangle->minY; y <= m_selectedRectangle->maxY; ++y)
				{
					if (map::Tile* tile = map.getTileIfExists(x, y))
					{
						tile->setColor(flat::video::Color::WHITE);
					}
				}
			}

			map::Zone* currentZone = m_currentZone.lock().get();
			FLAT_ASSERT(currentZone != nullptr);

			currentZone->removeRectangle(*m_selectedRectangle);

			m_selectedRectangle = nullptr;
		}
	}
}

void ZoneEditorMode::setCurrentZone(const std::shared_ptr<map::Zone>& currentZone)
{
	clearBrush();
	m_selectedRectangle = nullptr;
	m_currentZone = currentZone;
}

const std::shared_ptr<map::Zone>& ZoneEditorMode::addZone(const std::string& zoneName)
{
	map::Map& map = getMap();
	const std::shared_ptr<map::Zone>& zone = map.addZone(zoneName);
	return zone;
}

} // editor
} // states
} // game


