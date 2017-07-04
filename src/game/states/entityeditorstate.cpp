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

	// spawn the edited entity at the center of the Start zone
	std::shared_ptr<const entity::EntityTemplate> entityTemplate = getEntityTemplate(game, game.entityName);
	const map::Map& map = getMap();
	std::shared_ptr<map::Zone> zone;
	map.getZone("Start", zone);
	FLAT_ASSERT(zone != nullptr);
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
		m_componentRegistry.getEditorComponentsFilter()
	);

	FLAT_ASSERT(entity != nullptr);
	m_entity = entity->getHandle();

	setCameraCenter(position);
	setCameraZoom(2.f);

	game.lua->doFile("data/entityeditor/scripts/ui.lua");
}

void EntityEditorState::execute(Game& game)
{
	Super::execute(game);

	despawnEntities();
	const flat::time::Clock& clock = getClock();
	m_map.updateEntities(clock.getTime(), clock.getDT());
}

} // states
} // game
