#include "NHellScene.h"
#include "NGameObject.h"
#include "NPlayer.h"
#include "NTransform.h"
#include "NSpriteRenderer.h"

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
				= bg->AddComponent<Transform>();
			tr->SetPosition(Vector2(0, 0));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= bg->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			//sr->ImageLoad(L"../Resources/Hell.png");

			//AddGameObject(bg, enums::eLayerType::BackGround);
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

	void HellScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}
}