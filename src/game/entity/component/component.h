#ifndef GAME_ENTITY_COMPONENT_COMPONENT_H
#define GAME_ENTITY_COMPONENT_COMPONENT_H

#include <cstdint>
#include <memory>
#include <flat.h>

namespace game
{
class Game;
namespace entity
{
class Entity;
namespace component
{
class ComponentType;
class ComponentTemplate;

typedef uint8_t ComponentTypeId;

typedef uint32_t ComponentFlags;
enum { AllComponents = 0xFFFFFFFF };

class Component
{
	public:
		Component();
		virtual ~Component();
		
		inline void setOwner(Entity* owner) { m_owner = owner; }
		inline Entity* getOwner() { return m_owner; }
		inline const Entity* getOwner() const { return m_owner; }

		virtual void init() {}
		
		virtual void update(float currentTime, float elapsedTime) = 0;
		
		virtual bool isBusy() const { return false; }

		virtual const ComponentType& getComponentType() const = 0;

		static std::shared_ptr<ComponentTemplate> loadConfigFile(Game& game, lua_State* L, const std::string& entityTemplatePath);
		
	protected:
		Entity* m_owner;
};

#define DECLARE_COMPONENT_TYPE(T, name) \
	typedef Component Super; \
	private: \
		static std::shared_ptr<const ComponentType> type; \
	public: \
		inline static const char* getConfigName() { return #name; } \
		inline static const ComponentType* getType() { return type.get(); } \
		inline static void setType(const std::shared_ptr<const ComponentType>& type) { T::type = type; } \
		static ComponentFlags getFlag(); \
		const ComponentType& getComponentType() const override;

#define DEFINE_COMPONENT_TYPE(T) \
	static_assert(sizeof(ComponentType) > 0, "include componenttype.h"); \
	std::shared_ptr<const ComponentType> T::type; \
	ComponentFlags T::getFlag() { return type.get()->getComponentTypeFlag(); } \
	const ComponentType& T::getComponentType() const { return *type.get(); }


} // component
} // entity
} // game

#endif // GAME_ENTITY_COMPONENT_COMPONENT_H



