#include "Scenes/NPlayScene.h"
#include "GameObject/NGameObject.h"
#include "Contents/NPlayer.h"
#include "UI/NUIManager.h"
#include "Contents/NDemon.h"
#include "Component/Transform/NTransform.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "Helpers/NInput.h"
#include "Scenes/NSpaceScene.h"
#include "Scene/NSceneManager.h"
#include "Object/NObject.h"
#include "Resource/Texture/NTexture.h"
#include "Resource/NResources.h"
#include "Scripts/NPlayerScript.h"
#include "Component/Camera/NCamera.h"
#include "Renderer/NRenderer.h"
#include "Component/Animator/NAnimator.h"
#include "Component/Collider/NBoxCollider2D.h"
#include "Component/Collider/NCircleCollider2D.h"
#include "Collision/NCollisionManager.h"
#include "Contents/NTile.h"
#include "Component/TileMapRenderer/NTilemapRenderer.h"
#include "Component/RigidBody/NRigidbody.h"
#include "Contents/NFloor.h"
#include "Resource/Audio/NAudioClip.h"
#include "Component/Audio/NAudioListener.h"
#include "Component/Audio/NAudioSource.h"
#include "../../NuNuEngine_SOURCE/Graphics/GraphicDevice/NGraphicDevice_DX11.h"
#include "../../NuNuEngine_SOURCE/Component/SpriteRenderer/NSpriteRenderer.h"
#include "../../NuNuEngine_SOURCE/Resource/Material/NMaterial.h"
#include "../Scripts/NCameraScript.h"

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

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector3(0.0f, 0.0f, -10.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		cameraComp->SetSize(200.0f);

		CameraScript* cameraScript = camera->AddComponent<CameraScript>();
		renderer::mainCamera = cameraComp;

		mPlayer = object::Instantiate<Player>(enums::eLayerType::Player);
		object::DontDestroyOnLoad(mPlayer);

		SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
		sr->SetSprite(Resources::Find<graphics::Texture>(L"Player"));
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"SpaceScene");
		}
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


