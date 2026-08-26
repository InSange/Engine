#pragma once
#include "NEvent.h"
#include "../GameObject/NGameObject.h"
#include "../Scene/NScene.h"



namespace NuNu
{
	class GameObjectEvent : public Event
	{
	public:
		GameObject::eState GetState() const { return State; }
		EVENT_CLASS_CATEGORY(EventCategoryGameObject | EventCategoryGame)

	protected:
		GameObjectEvent(const GameObject::eState state)
			: State(state) {
		}

		GameObject::eState State;
	};

	class GameObjectCreatedEvent : public GameObjectEvent
	{
	public:
		GameObjectCreatedEvent(GameObject* gameObject, Scene* scene)
			: GameObjectEvent(GameObject::eState::Created)
			, mGameObject(gameObject)
			, mScene(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GameObjectCreatedEvent";
			return ss.str();
		}

		GameObject* GetGameObject() const { return mGameObject; }
		Scene* GetScene() const { return mScene; }

		EVENT_CLASS_TYPE(GameObjectCreated)

	private:
		GameObject* mGameObject;
		Scene* mScene;
	};

	class GameObjectDestroyedEvent : public GameObjectEvent
	{
	public:
		GameObjectDestroyedEvent(GameObject* gameObject, Scene* scene)
			: GameObjectEvent(GameObject::eState::Destroyed)
			, mGameObject(gameObject)
			, mScene(scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GameObjectDestroyedEvent";
			return ss.str();
		}

		GameObject* GetGameObject() const { return mGameObject; }
		Scene* GetScene() const { return mScene; }

		EVENT_CLASS_TYPE(GameObjectDestroyed)

	private:
		GameObject* mGameObject;
		Scene* mScene;
	};
}