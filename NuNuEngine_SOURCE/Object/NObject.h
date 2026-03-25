#pragma once
#include "Component/Component/NComponent.h"
#include "Layer/NLayer.h"
#include "GameObject/NGameObject.h"
#include "Scene/NSceneManager.h"
#include "Scene/NScene.h"
#include "Component/Transform/NTransform.h"
#include "../High Level Interface/NApplication.h"
#include "../Event/NGameObjectEvent.h"
#include "../Scene/NSceneManager.h"

extern NuNu::Application application;

namespace NuNu::object
{
	template <typename T>
	static T* Instantiate(eLayerType type)
	{
		T* gameObject = new T();
		gameObject->SetLayerType(type);

		Scene* activeScene = SceneManager::GetActiveScene();
		SceneManager::PushEvent(new NuNu::GameObjectCreatedEvent(gameObject, activeScene));

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Vector3 position)
	{
		T* gameObject = new T();
		gameObject->SetLayerType(type);

		Transform* tr = gameObject->template GetComponent<Transform>();
		tr->SetPosition(position);

		Scene* activeScene = SceneManager::GetActiveScene();
		SceneManager::PushEvent(new NuNu::GameObjectCreatedEvent(gameObject, activeScene));

		return gameObject;
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		activeScene->EraseGameObject(gameObject);

		Scene* dontDestroyOnLoad = SceneManager::GetDontDestroyOnLoad();
		dontDestroyOnLoad->AddGameObject(gameObject, gameObject->GetLayerType());
	}

	static void Destroy(GameObject* gameObject)
	{
		if (gameObject != nullptr)
			gameObject->death();

		Scene* activeScene = SceneManager::GetActiveScene();
		SceneManager::PushEvent(new NuNu::GameObjectDestroyedEvent(gameObject, activeScene));
	}
}
