#ifndef GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENTTEMPLATE_H

#include "../../componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace detection
{

class DetectionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		inline float getVisionRange() const { return m_visionRange; }

	private:
		float m_visionRange;
};

} // detection
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_DETECTION_DETECTIONCOMPONENTTEMPLATE_H


