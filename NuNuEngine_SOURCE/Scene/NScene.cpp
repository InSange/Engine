#include "Scene/NScene.h"
#include "Collision/NCollisionManager.h"
#include "NSceneManager.h"

namespace NuNu
{
	Scene::Scene()
		: mLayers{}
	{
		createLayers();
	}

	Scene::~Scene()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;
			delete layer;
			layer = nullptr;
		}
	}

	void Scene::Initialize()
	{
		const std::wstring& sceneName = GetName();
		SceneManager::SetActiveScene(sceneName);
	}

	void Scene::Update()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->LateUpdate();
		}
	}

	void Scene::Render()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->Render();
		}
	}

	void Scene::EndOfFrame()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->EndOfFrame();
		}
	}

	void Scene::OnEnter()
	{
	}

	void Scene::OnExit()
	{
		CollisionManager::Clear();
	}

	void Scene::AddGameObject(GameObject* gameObj, eLayerType type)
	{
		if (gameObj == nullptr) return;
		mLayers[static_cast<UINT>(type)]->AddGameObject(gameObj);
	}

	void Scene::EraseGameObject(GameObject* gameObj)
	{
		if (gameObj == nullptr)
			return;

		eLayerType layerType = gameObj->GetLayerType();
		mLayers[static_cast<UINT>(layerType)]->EraseGameObject(gameObj);
	}

	void Scene::createLayers()
	{
		mLayers.resize(static_cast<UINT>(enums::eLayerType::Max));
		for (size_t i = 0; i < static_cast<UINT>(enums::eLayerType::Max); i++)
		{
			mLayers[i] = new Layer();
			//mLayers[i]->SetType(enums::eLayerType(i));
		}
	}
}

