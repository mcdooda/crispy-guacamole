#include "entity/component/components/sample/samplecomponent.h"
#include "entity/entity.h"
#include "map/map.h"

namespace game::entity::component::sample
{

void SampleComponent::update(float currentTime, float elapsedTime)
{
	for (const auto& music: m_samples)
	{

	}
}


void SampleComponent::playSample(flat::Game& game, const std::string& filename, int numLoops)
{
	flat::audio::Sample& sample = m_samples.emplace_back(game.audio->getChunk(filename));
	sample.play(numLoops);
}

} // game::entity::component::sample
