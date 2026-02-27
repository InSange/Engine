#include "NScene.h"

namespace NuNu
{
	Scene::Scene()
		: mLayers{}
	{
		createLayers();
	}
	Scene::~Scene()
	{
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
	void Scene::OnEnter()
	{
	}
	void Scene::OnExit()
	{
	}
	void Scene::AddGameObject(GameObject* gameObj, enums::eLayerType type)
	{
		if (gameObj == nullptr) return;
		mLayers[(UINT)type]->AddGameObject(gameObj);
	}
	void Scene::createLayers()
	{
		mLayers.resize((UINT)enums::eLayerType::Max);
		for (size_t i = 0; i < (UINT)enums::eLayerType::Max; i++)
		{
			mLayers[i] = new Layer();
			mLayers[i]->SetType(enums::eLayerType(i));
		}
	}
}