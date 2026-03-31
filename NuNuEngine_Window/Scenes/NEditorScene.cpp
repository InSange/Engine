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
#include "Component/Collider3D/NCollider3D.h"

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
		// 카메라 y = 바닥(0) + eyeHeight(1.7) = 1.7 로 스폰
		GameObject* camera = object::Instantiate<GameObject>(eLayerType::None, Vector3(0.0f, 1.7f, -10.0f));

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

		// 물리 바닥 (보이지 않는 AABB, 전체 영역 커버)
		GameObject* floor = object::Instantiate<GameObject>(eLayerType::None, Vector3(0, -0.5f, 0));
		Collider3D* floorCol = floor->AddComponent<Collider3D>();
		floorCol->mHalfExtents = math::Vector3(50.0f, 0.5f, 50.0f);

		// KayKit Platformer: 4×4 바닥 타일 격자 (총 16×16 유닛)
		// row 0      : Safe Area (출발) — 초록 tint
		// row 1 ~ 2  : Danger Area     — 빨강 tint
		// row 3      : Safe Area (도착) — 초록 tint
		{
			constexpr float tileSize = 4.0f;
			constexpr int   gridN    = 4;
			const float     start    = -(gridN * tileSize) * 0.5f; // -8

			for (int row = 0; row < gridN; row++)
			{
				const bool isSafe = (row == 0 || row == gridN - 1);

				for (int col = 0; col < gridN; col++)
				{
					float tx = start + col * tileSize + tileSize * 0.5f;
					float tz = start + row * tileSize + tileSize * 0.5f;

					GameObject* tile = object::Instantiate<GameObject>(eLayerType::None, Vector3(tx, 0.0f, tz));
					MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
					mr->SetMesh3D(Resources::Find<Mesh3D>(L"FloorWood4x4"));
					mr->SetShader3D(Resources::Find<graphics::Shader>(L"Mesh3DShader"));
					mr->SetTextureSRV(renderer::platformerTexSRV);

					if (isSafe) mr->SetColor(0.4f, 1.0f, 0.4f); // Safe  — 초록
					else        mr->SetColor(1.0f, 0.4f, 0.4f); // Danger — 빨강
				}
			}
		}

		// KayKit Platformer: 외벽 (barrier_4x1x1) — 남/북/동/서 각 4개씩
		{
			constexpr float wallLen  = 4.0f;
			constexpr int   wallN    = 4;
			const float     edgeDist = 8.0f; // 바닥 끝까지
			const float     wallY    = 0.5f; // 벽 높이 중심

			for (int i = 0; i < wallN; i++)
			{
				float offset = -6.0f + i * wallLen;

				// 북쪽 벽 (z = +edgeDist)
				{
					GameObject* wall = object::Instantiate<GameObject>(eLayerType::None, Vector3(offset, wallY, edgeDist));
					MeshRenderer* mr = wall->AddComponent<MeshRenderer>();
					mr->SetMesh3D(Resources::Find<Mesh3D>(L"Barrier4x1x1"));
					mr->SetShader3D(Resources::Find<graphics::Shader>(L"Mesh3DShader"));
					mr->SetTextureSRV(renderer::platformerTexSRV);
					Collider3D* col = wall->AddComponent<Collider3D>();
					col->mHalfExtents = math::Vector3(2.0f, 0.5f, 0.5f);
				}
				// 남쪽 벽 (z = -edgeDist)
				{
					GameObject* wall = object::Instantiate<GameObject>(eLayerType::None, Vector3(offset, wallY, -edgeDist));
					MeshRenderer* mr = wall->AddComponent<MeshRenderer>();
					mr->SetMesh3D(Resources::Find<Mesh3D>(L"Barrier4x1x1"));
					mr->SetShader3D(Resources::Find<graphics::Shader>(L"Mesh3DShader"));
					mr->SetTextureSRV(renderer::platformerTexSRV);
					Collider3D* col = wall->AddComponent<Collider3D>();
					col->mHalfExtents = math::Vector3(2.0f, 0.5f, 0.5f);
				}
			}
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