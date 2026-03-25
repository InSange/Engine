#pragma once
#include "NEvent.h"
#include "../GameObject/NGameObject.h"
#include "../Scene/NScene.h"



namespace NuNu
{
	class GameObjectEvent : public Event
	{
	public:
		GameObject::eState GetState() const { return mState; }
		EVENT_CLASS_CATEGORY(EventCategoryGameObject | EventCategoryGame)

	protected:
		GameObjectEvent(const GameObject::eState state)
			: mState(state) {
		}

		GameObject::eState mState;
	};

	class GameObjectCreatedEvent : public GameObjectEvent
	{
	public:
		GameObjectCreatedEvent(GameObject* gameObject, Scene* scene)
			: GameObjectEvent(GameObject::eState::Created)
			, mGameObject(gameObject) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GameObjectCreatedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(GameObjectCreated)

	private:
		GameObject* mGameObject;
		Scene* mScene;
	};

	class GameObjectDestroyedEvent : public GameObjectEvent
	{
	public:
		GameObjectDestroyedEvent(GameObject* gameObject, Scene* scene)
			: GameObjectEvent(GameObject::eState::Destroyed) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GameObjectDestroyedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(GameObjectDestroyed)

	private:
		GameObject* mGameObject;
		Scene* mScene;
	};
}