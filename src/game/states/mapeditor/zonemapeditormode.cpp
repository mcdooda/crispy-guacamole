#include "zonemapeditormode.h"
#include "../../map/map.h"
#include "../../game.h"

namespace game
{
namespace states
{
namespace editor
{

ZoneMapEditorMode::ZoneMapEditorMode(Game& game) : Super(game)
{

}

void ZoneMapEditorMode::enter(MapEditorState& mapEditorState)
{
	m_drawingRectangle = false;
	m_selectedRectangle = nullptr;
}

void ZoneMapEditorMode::clearBrush(MapEditorState& mapEditorState) const
{
	map::Zone* previousZone = m_currentZone.lock().get();

	if (previousZone != nullptr)
	{
		previousZone->eachTileIfExists([previousZone](const map::Tile* tile)
		{
			// TODO: fix this shit
			const_cast<map::Tile*>(tile)->setColor(flat::video::Color::WHITE);
		});
	}

	if (m_drawingRectangle
		&& (m_currentRectangle.minX < m_currentRectangle.maxX || m_currentRectangle.minY < m_currentRectangle.maxY))
	{
		map::Map& map = mapEditorState.getMap();
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
		map::Map& map = mapEditorState.getMap();
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

void ZoneMapEditorMode::displayBrush(MapEditorState& mapEditorState) const
{
	map::Zone* currentZone = m_currentZone.lock().get();
	if (currentZone != nullptr)
	{
		currentZone->eachTileIfExists([currentZone](const map::Tile* tile)
		{
			// TODO: fix this shit
			const_cast<map::Tile*>(tile)->setColor(currentZone->getColor());
		});
	}

	if (m_drawingRectangle
		&& (m_currentRectangle.minX < m_currentRectangle.maxX || m_currentRectangle.minY < m_currentRectangle.maxY))
	{
		map::Map& map = mapEditorState.getMap();
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
		map::Map& map = mapEditorState.getMap();
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

void ZoneMapEditorMode::updateBrushTiles(MapEditorState& mapEditorState)
{
	if (m_currentZone.expired())
	{
		return;
	}

	const auto& mouse = m_game.input->mouse;

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

void ZoneMapEditorMode::handleShortcuts(MapEditorState& mapEditorState)
{
	const auto& keyboard = m_game.input->keyboard;

	if (keyboard->isJustPressed(K(DELETE)))
	{
		if (m_selectedRectangle != nullptr)
		{
			map::Map& map = mapEditorState.getMap();
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

void ZoneMapEditorMode::setCurrentZone(MapEditorState& mapEditorState, const std::shared_ptr<map::Zone>& currentZone)
{
	clearBrush(mapEditorState);
	m_selectedRectangle = nullptr;
	m_currentZone = currentZone;
}

const std::shared_ptr<map::Zone>& ZoneMapEditorMode::addZone(MapEditorState& mapEditorState, const std::string& zoneName)
{
	map::Map& map = mapEditorState.getMap();
	const std::shared_ptr<map::Zone>& zone = map.addZone(zoneName);
	return zone;
}

bool ZoneMapEditorMode::removeZone(MapEditorState& mapEditorState, const std::string& zoneName)
{
	map::Map& map = mapEditorState.getMap();
	return map.removeZone(zoneName);
}

} // editor
} // states
} // game


