#include "NEditorScene.h"
#include "NSpaceScene.h"
#include "../Contents/NPlayer.h"
#include "UI/NUIManager.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "GameObject/NGameObject.h"
#include "Object/NObject.h"
#include "Resource/Texture/NTexture.h"
#include "Resource/NResources.h"
#include "../Scripts/NPlayerScript.h"
#include "Component/Camera/NCamera.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Component/Camera/NSceneCamera.h"
#include "Component/MeshRenderer/NMeshRenderer.h"
#include "Resource/Mesh3D/NMesh3D.h"
#include "Component/CharacterController/NCharacterController.h"

namespace NuNu
{
	EditorScene::EditorScene()
	{
	}

	EditorScene::~EditorScene()
	{
	}

	void EditorScene::Initialize()
	{
		Scene::Initialize();

		// main camera
		GameObject* camera = object::Instantiate<GameObject>(eLayerType::None, Vector3(0.0f, 0.0f, -10.0f));

		SceneCamera* cameraComp = camera->AddComponent<SceneCamera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);

		camera->AddComponent<CharacterController>();

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

		// Phase 1-3 테스트: Barbarian FBX 렌더링
		{
			GameObject* meshObj = object::Instantiate<GameObject>(eLayerType::None, Vector3(0.0f, 0.0f, 0.0f));
			MeshRenderer* mr = meshObj->AddComponent<MeshRenderer>();
			mr->SetMesh3D(Resources::Find<Mesh3D>(L"Barbarian"));
			mr->SetShader3D(Resources::Find<graphics::Shader>(L"Mesh3DShader"));
			mr->SetTextureSRV(renderer::barbarianTexSRV);
		}
	}

	void EditorScene::Update()
	{
		Scene::Update();
	}

	void EditorScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void EditorScene::Render()
	{
		Scene::Render();
	}

	void EditorScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void EditorScene::OnExit()
	{
		Scene::OnExit();
	}
}