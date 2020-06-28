#ifndef GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H
#define GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H

#include <flat.h>
#include "spritecomponenttemplate.h"
#include "../../component.h"

namespace game
{
namespace entity
{
class EntityUpdater;

namespace component
{
namespace sprite
{

class SpriteComponent : public ComponentImpl<SpriteComponentTemplate>
{
	public:
		struct AttachedSprite : public map::MapObject
		{
			flat::render::Sprite sprite;
			flat::Matrix4 relativeTransform;

			flat::render::BaseSprite& getSprite() override;
			const flat::render::ProgramSettings& getProgramSettings() const override;
			bool isEntity() const override;
		};

	public:
		inline static const char* getConfigName() { return "sprite"; }
		inline static const char* getVisualName() { return "Sprite"; }

		void init() override;
		void deinit() override;

		void update(float currentTime, float elapsedTime) override;

		bool isBusy() const override;

		void setCycleAnimation(const AnimationDescription& animationDescription);
		bool setCycleAnimationByName(const std::string& animationName);
		void clearCycleAnimation();

		void setCycleAnimated(bool isCycleAnimated);
		void resetCycleAnimation();

		void playAnimation(const AnimationDescription& animationDescription, int numLoops = flat::render::AnimatedSprite::INFINITE_LOOP, bool preventBusy = true);
		bool playAnimationByName(const std::string& animationName, int numLoops, bool preventBusy);

		bool setAnimationProgress(const std::string& animationName, float value);

		bool getAttachPoint(const std::string& attachPointName, flat::Vector3& attachPoint) const;

		inline const AnimationDescription* getCurrentAnimationDescription() const { return m_currentAnimationDescription; }
		inline bool hasInfiniteLoop() const { return m_sprite.hasInfiniteLoop(); }

		void attachSprite(const flat::render::Sprite& otherSprite);
		void pushAttachedSprites(std::vector<const map::MapObject*>& objects) const;

		FLAT_DEBUG_ONLY(void debugDraw(debug::DebugDisplay& debugDisplay) const override;)

	private:
		bool selected();
		bool deselected();

		bool addedToMap(Entity* entity, map::Map* map, EntityUpdater* entityUpdater);
		bool removedFromMap(Entity* entity);

		bool updateSpritePosition(const flat::Vector3& position);

		void updateAttachedSprites();

	private:
		flat::render::AnimatedSprite m_sprite;
		std::vector<AttachedSprite> m_attachedSprites;

		const AnimationDescription* m_cycleAnimationDescription;
		const AnimationDescription* m_currentAnimationDescription;
		bool m_isCycleAnimated;
		bool m_preventBusy;
};

} // sprite
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_SPRITE_SPRITECOMPONENT_H


