#include "NPlayScene.h"
#include "NGameObject.h"
#include "NPlayer.h"
#include "NTransform.h"
#include "NSpriteRenderer.h"
#include "NInput.h"
#include "NSpaceScene.h"
#include "NSceneManager.h"
#include "NObject.h"
#include "NTexture.h"
#include "NResources.h"

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

		GameObject* bgObj = object::Instantiate<GameObject>(enums::eLayerType::BackGround, Vector2(100.0f, 100.0f));
		SpriteRenderer* sr = bgObj->AddComponent<SpriteRenderer>();
		sr->SetTexture(Resources::Find<graphics::Texture>(L"BG"));

		for (size_t i = 0; i < 5; i++)
		{
			GameObject* obj = object::Instantiate<GameObject>(enums::eLayerType::UI, Vector2(rand() % 1600, rand() % 900));

			sr = obj->AddComponent<SpriteRenderer>();
			sr->SetTexture(Resources::Find<graphics::Texture>(L"TestIcon"));
		}

		bg = object::Instantiate<Player>(enums::eLayerType::Player, Vector2(800, 450));
		sr = bg->AddComponent<SpriteRenderer>();
		sr->SetTexture(Resources::Find<graphics::Texture>(L"Logi"));
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
		/*		Transform* tr = bg->GetComponent<Transform>();
				tr->SetPosition(Vector2(0, 0));*/
	}
}