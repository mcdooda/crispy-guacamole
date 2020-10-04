#ifndef GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENTTEMPLATE_H
#define GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENTTEMPLATE_H

#include "collisionbox.h"
#include "entity/component/componenttemplate.h"

namespace game
{
namespace entity
{
namespace component
{
namespace collision
{

class CollisionComponentTemplate : public ComponentTemplate
{
	public:
		void load(Game& game, lua_State* L, const std::filesystem::path& entityTemplatePath) override final;

		const CollisionBox& getCollisionBox() const { return m_collisionBox; }

		inline float getRadius() const { return m_collisionBox.getRadius(); }

		inline bool shouldSeparateFromOtherEntities() const { return m_separate; }
		inline bool shouldSeparateFromSameType() const { return m_separateSameType; }

		inline bool shouldSeparateFromTiles() const { return m_separateFromTiles; }

	private:
		CollisionBox m_collisionBox;
		bool m_separate;
		bool m_separateSameType;
		bool m_separateFromTiles;
};

} // collision
} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COLLISION_COLLISIONCOMPONENTTEMPLATE_H


