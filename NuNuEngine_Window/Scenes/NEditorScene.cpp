#include "NEditorScene.h"
#include "NSpaceScene.h"
#include "../Contents/NPlayer.h"
#include "../../NuNuEngine_SOURCE/UI/NUIManager.h"
#include "../../NuNuEngine_SOURCE/Component/SpriteRenderer/NSpriteRenderer.h"
#include "../../NuNuEngine_SOURCE/GameObject/NGameObject.h"
#include "../../NuNuEngine_SOURCE/Object/NObject.h"
#include "../../NuNuEngine_SOURCE/Resource/Texture/NTexture.h"
#include "../../NuNuEngine_SOURCE/Resource/NResources.h"
#include "../Scripts/NPlayerScript.h"
#include "../../NuNuEngine_SOURCE/Component/Camera/NCamera.h"
#include "../../NuNuEngine_SOURCE/High Level Interface/Renderer/NRenderer.h"
#include "../../NuNuEngine_SOURCE/Component/Camera/NSceneCamera.h"

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