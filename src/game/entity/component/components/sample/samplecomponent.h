#ifndef GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H

#include "game.h"
#include "samplecomponenttemplate.h"
#include "../../component.h"

namespace game::entity::component::sample
{

class SampleComponent : public ComponentImpl<SampleComponentTemplate>
{
	public:
		inline static const char* getConfigName() { return "sample"; }
		inline static const char* getVisualName() { return "Sample"; }

		void update(float currentTime, float elapsedTime) override;
		void playSample(flat::Game& game, const std::string& filename, int numloops);

	private:
		std::vector<flat::audio::Sample> m_samples;
};

} // game::entity::component::sample

#endif // GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H
