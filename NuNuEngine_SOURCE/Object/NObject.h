#pragma once
#include "Component/Component/NComponent.h"
#include "Layer/NLayer.h"
#include "GameObject/NGameObject.h"
#include "Scene/NSceneManager.h"
#include "Scene/NScene.h"
#include "Component/Transform/NTransform.h"

namespace NuNu::object
{
	template <typename T>
	static T* Instantiate(enums::eLayerType type)
	{
		T* gameObject = new T();
		gameObject->SetLayerType(type);
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);

		layer->AddGameObject(gameObject);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, math::Vector3 position)
	{
		T* gameObject = new T();
		gameObject->SetLayerType(type);
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);

		layer->AddGameObject(gameObject);

		Transform* tr = gameObject->GetComponent<Transform>();
		tr->SetPosition(position);

		return gameObject;
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		activeScene->EraseGameObject(gameObject);

		Scene* dontDestroyOnLoad = SceneManager::GetDontDestroyOnLoad();
		dontDestroyOnLoad->AddGameObject(gameObject, gameObject->GetLayerType());
	}
}
