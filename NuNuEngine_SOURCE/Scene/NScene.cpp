#include "Scene/NScene.h"
#include "NCollisionManager.h"

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
		/*GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None);
		Camera* cameraComp = camera->AddComponent<Camera>();
		camera->AddComponent<PlayerScript>();*/

		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->Initialize();
		}
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

	void Scene::Render(HDC hdc)
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->Render(hdc);
		}
	}

	void Scene::Destroy()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr) continue;

			layer->Destroy();
		}
	}

	void Scene::OnEnter()
	{
	}

	void Scene::OnExit()
	{
		CollisionManager::Clear();
	}

	void Scene::AddGameObject(GameObject* gameObj, enums::eLayerType type)
	{
		if (gameObj == nullptr) return;
		mLayers[(UINT)type]->AddGameObject(gameObj);
	}

	void Scene::EraseGameObject(GameObject* gameObj)
	{
		eLayerType layerType = gameObj->GetLayerType();
		mLayers[(UINT)layerType]->EraseGameObject(gameObj);
	}

	void Scene::createLayers()
	{
		mLayers.resize((UINT)enums::eLayerType::Max);
		for (size_t i = 0; i < (UINT)enums::eLayerType::Max; i++)
		{
			mLayers[i] = new Layer();
			//mLayers[i]->SetType(enums::eLayerType(i));
		}
	}
}
