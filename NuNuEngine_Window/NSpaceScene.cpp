#include "NSpaceScene.h"
#include "NPlayer.h"
#include "NTransform.h"
#include "NSpriteRenderer.h"
#include "NInput.h"
#include "NPlayScene.h"
#include "NSceneManager.h"

namespace NuNu
{
	SpaceScene::SpaceScene()
	{
	}
	SpaceScene::~SpaceScene()
	{
	}
	void SpaceScene::Initialize()
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
			sr->ImageLoad(L"../Resources/Space.jpg");

			AddGameObject(bg, eLayerType::BackGround);
		}
	}
	void SpaceScene::Update()
	{
		Scene::Update();
	}
	void SpaceScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"PlayScene");
		}
	}
	void SpaceScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
		wchar_t str[50] = L"Hell Scene";
		TextOut(hdc, 0, 0, str, 10);
	}
}