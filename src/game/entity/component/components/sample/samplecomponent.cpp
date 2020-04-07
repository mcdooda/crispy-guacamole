#include "entity/component/components/sample/samplecomponent.h"
#include "entity/entity.h"
#include "map/map.h"

namespace game::entity::component::sample
{

void SampleComponent::playSample(Game& game, const std::string& filename, int numLoops)
{
	std::shared_ptr<const flat::audio::Chunk> chunk = game.audio->getChunk(filename);
	game.audio->playChunk(chunk);
}

} // game::entity::component::sample
