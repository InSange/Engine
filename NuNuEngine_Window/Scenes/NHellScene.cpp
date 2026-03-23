#include "Scenes/NHellScene.h"
#include "GameObject/NGameObject.h"
#include "Contents/NPlayer.h"
#include "Component/Transform/NTransform.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"

namespace NuNu
{
	HellScene::HellScene()
	{
	}

	HellScene::~HellScene()
	{
	}

	void HellScene::Initialize()
	{
		{
			Player* bg = new Player();
			Transform* tr
				= bg->GetComponent<Transform>();
			tr->SetPosition(Vector3(0, 0, 0));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= bg->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			//sr->ImageLoad(L"../Resources/Hell.png");

			AddGameObject(bg, enums::eLayerType::BackGround);
		}
	}

	void HellScene::Update()
	{
		Scene::Update();
	}

	void HellScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void HellScene::Render()
	{
		Scene::Render();
	}
	void HellScene::OnEnter()
	{
		Scene::OnEnter();
	}
	void HellScene::OnExit()
	{
		Scene::OnExit();
	}
}

