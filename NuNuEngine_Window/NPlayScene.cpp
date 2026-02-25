#include "NPlayScene.h"
#include "NGameObject.h"
#include "NPlayer.h"
#include "NTransform.h"
#include "NSpriteRenderer.h"
#include "NInput.h"
#include "NSpaceScene.h"
#include "NSceneManager.h"

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
		{
			bg = new Player();
			Transform* tr
				= bg->AddComponent<Transform>();
			tr->SetPosition(Vector2(0, 0));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= bg->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			sr->ImageLoad(L"../Resources/BlackHole.jpg");

			AddGameObject(bg, eLayerType::BackGround);
		}

		for (size_t i = 0; i < 5; i++)
		{
			GameObject* obj = new GameObject();
			Transform* tr
				= obj->AddComponent<Transform>();
			tr->SetPosition(Vector2(rand()%500 + 200, rand()%500 + 100));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= obj->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			sr->ImageLoad(L"../Resources/testIcon.png");

			AddGameObject(obj, eLayerType::UI);
		}

		{
			Player* pl = new Player();
			Transform* tr
				= pl->AddComponent<Transform>();
			tr->SetPosition(Vector2(800, 450));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= pl->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			sr->ImageLoad(L"../Resources/Logi.png");

			AddGameObject(pl, eLayerType::Player);
		}

		for (size_t i = 0; i < 5; i++)
		{
			GameObject* obj = new GameObject();
			Transform* tr
				= obj->AddComponent<Transform>();
			tr->SetPosition(Vector2(rand() % 1600, rand() % 900));

			tr->SetName(L"TR");

			SpriteRenderer* sr
				= obj->AddComponent<SpriteRenderer>();
			sr->SetName(L"SR");
			sr->ImageLoad(L"../Resources/tree.png");

			AddGameObject(obj, eLayerType::UI);
		}
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
	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
		wchar_t str[50] = L"Play Scene";
		TextOut(hdc, 0, 0, str, 10);
	}
	void PlayScene::OnEnter()
	{
	}
	void PlayScene::OnExit()
	{
		Transform* tr = bg->GetComponent<Transform>();
		tr->SetPosition(Vector2(0, 0));
	}
}