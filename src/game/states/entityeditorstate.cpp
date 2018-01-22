#include "entityeditorstate.h"
#include "../game.h"
#include "../map/zone.h"
#include "../map/tile.h"

namespace game
{
namespace states
{

void EntityEditorState::enter(Game& game)
{
	Super::enter(game);

	game.lua->doFile("data/entityeditor/scripts/init.lua");

	spawnEntity(game);
#ifdef FLAT_DEBUG
	m_debuggedComponentFlags = 0;
#endif

	setCameraCenter(m_entity.getEntity()->getPosition());
	setCameraZoom(2.f);

	game.lua->doFile("data/entityeditor/scripts/ui.lua");
}

void EntityEditorState::execute(Game& game)
{
	Super::execute(game);

	despawnEntities();
	if (!m_entity.isValid())
	{
		spawnEntity(game);

#ifdef FLAT_DEBUG
		entity::Entity* entity = m_entity.getEntity();
		entity->setDebug(m_debuggedComponentFlags != 0);
		entity->setDebuggedComponentFlags(m_debuggedComponentFlags);
#endif
	}

	const flat::time::Clock& clock = getClock();
	m_map.updateEntities(clock.getTime(), clock.getDT());

#ifdef FLAT_DEBUG
	entity::Entity* entity = m_entity.getEntity();
	if (entity != nullptr)
	{
		entity::component::ComponentFlags debuggedComponentFlags = entity->getDebuggedComponentFlags();
		if (debuggedComponentFlags != 0)
		{
			m_debuggedComponentFlags = debuggedComponentFlags;
		}
	}
#endif
}

void EntityEditorState::spawnEntity(Game& game)
{
	// spawn the edited entity at the center of the Start zone
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, game.entityName);
	const map::Map& map = getMap();
	std::shared_ptr<map::Zone> zone;
	map.getZone("Start", zone);
	FLAT_ASSERT_MSG(zone != nullptr, "The entity editor map must have a 'Start' zone on a walkable tile");
	flat::Vector2 zoneCenter = zone->getCenter();
	const map::Tile* centerTile = map.getTileIfWalkable(zoneCenter.x, zoneCenter.y);
	FLAT_ASSERT_MSG(centerTile != nullptr, "The entity editor map must have a 'Start' zone on a walkable tile");
	flat::Vector3 position(zoneCenter, centerTile->getZ());

	entity::Entity* entity = spawnEntityAtPosition(
		game,
		entityTemplate,
		position,
		0.f,
		0.f,
		entity::component::AllComponents,
		m_componentRegistry.getEntityEditorComponentsFilter()
	);

	FLAT_ASSERT(entity != nullptr);
	m_entity = entity->getHandle();
	entitySpawned(entity);
}

} // states
} // game

