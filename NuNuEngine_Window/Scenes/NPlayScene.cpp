#include "Scenes/NPlayScene.h"
#include "GameObject/NGameObject.h"
#include "Contents/NPlayer.h"
#include "UI/NUIManager.h"
#include "Contents/NDemon.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "Scenes/NSpaceScene.h"
#include "Object/NObject.h"
#include "Resource/Texture/NTexture.h"
#include "Resource/NResources.h"
#include "Scripts/NPlayerScript.h"
#include "Component/Camera/NCamera.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Component/Camera/NSceneCamera.h"

namespace NuNu
{
	PlayScene::PlayScene()
	{
	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{
		Scene::Initialize();

		GameObject* camera = object::Instantiate<GameObject>(eLayerType::None, Vector3(0.0f, 0.0f, -10.0f));
		SceneCamera* cameraComp = camera->AddComponent<SceneCamera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		cameraComp->SetSize(200.0f);

		renderer::mainCamera = cameraComp;

		for (size_t i = 0; i < 1; i++)
		{
			GameObject* player = object::Instantiate<Player>(eLayerType::Player);
			SpriteRenderer* sr = player->AddComponent<SpriteRenderer>();
			sr->SetSprite(Resources::Find<Texture>(L"Player"));

			player->AddComponent<PlayerScript>();

			if (renderer::selectedObject == nullptr)
				renderer::selectedObject = player;
		}
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{

	}

	void PlayScene::Render()
	{
		Scene::Render();
	}

	void PlayScene::OnEnter()
	{
		Scene::OnEnter();

		// CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);
		// CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Floor, true);

		// UIManager::Push(eUIType::Button);
	}

	void PlayScene::OnExit()
	{
		/*		Transform* tr = bg->GetComponent<Transform>();
				tr->SetPosition(Vector2(0, 0));*/

		// UIManager::Pop(eUIType::Button);


		Scene::OnExit();
	}
}


