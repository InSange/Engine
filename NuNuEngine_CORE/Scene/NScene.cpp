#include "Scene/NScene.h"
#include "Collision/NCollisionManager.h"
#include "Resource/Material/NMaterial.h"
#include "NSceneManager.h"
#include "Component/Transform/NTransform.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"

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
		for (Camera* camera : mCameras)
		{
			if (camera == nullptr) continue;

			Matrix viewMatrix = camera->GetViewMatrix();
			Matrix projectionMatrix = camera->GetProjectionMatrix();
			Vector3 cameraPos = camera->GetOwner()->GetComponent<Transform>()->GetPosition();

			std::vector<GameObject*> opaqueList = {};
			std::vector<GameObject*> cutoutList = {};
			std::vector<GameObject*> transparentList = {};

			// collect randerables(game objects)
			// renderer::CollectRenderables(this, opaqueList, cutoutList, transparentList);

			//// soring renderables by distance (between camera and game object)
			//renderer::SortByDistance(opaqueList, cameraPos, true);
			//renderer::SortByDistance(cutoutList, cameraPos, true);
			//renderer::SortByDistance(transparentList, cameraPos, false);

			//// render game objects
			//renderer::RenderRenderables(opaqueList, viewMatrix, projectionMatrix);
			//renderer::RenderRenderables(cutoutList, viewMatrix, projectionMatrix);
			//renderer::RenderRenderables(transparentList, viewMatrix, projectionMatrix);

			graphics::GetDevice()->BeginGameRenderTarget();
			renderer::RenderSceneFromCamera(this, camera);
			graphics::GetDevice()->EndGameRenderTarget();
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

	void Scene::AddCamera(Camera* camera)
	{
		if (camera == nullptr)
			return;

		mCameras.push_back(camera);
	}

	void Scene::RemoveCamera(Camera* camera)
	{
		if (camera == nullptr)
			return;

		auto iter
			= std::find(mCameras.begin(), mCameras.end(), camera);

		if (iter != mCameras.end())
			mCameras.erase(iter);
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

