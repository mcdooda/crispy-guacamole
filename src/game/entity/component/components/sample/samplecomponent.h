#ifndef GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H

#include <filesystem>
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
		inline static bool requiresUpdate() { return false; }

		void playSample(Game& game, const std::filesystem::path& filename, int numloops);
};

} // game::entity::component::sample

#endif // GAME_ENTITY_COMPONENT_SAMPLE_SAMPLECOMPONENT_H
