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
#include "NPlayerScript.h"
#include "NCamera.h"
#include "NRenderer.h"

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
		//Scene::Initialize();
		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector2(812.0f, 470.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComp;

		//camera->AddComponent<PlayerScript>();

		GameObject* bgObj = object::Instantiate<GameObject>(enums::eLayerType::BackGround, Vector2::Zero);
		SpriteRenderer* sr = bgObj->AddComponent<SpriteRenderer>();
		sr->SetTexture(Resources::Find<graphics::Texture>(L"BG"));

		for (size_t i = 0; i < 5; i++)
		{
			GameObject* obj = object::Instantiate<GameObject>(enums::eLayerType::UI, Vector2(rand() % 1600, rand() % 900));

			sr = obj->AddComponent<SpriteRenderer>();
			sr->SetTexture(Resources::Find<graphics::Texture>(L"TestIcon"));
		}

		mPlayer = object::Instantiate<Player>(enums::eLayerType::Player, Vector2(0, 0));
		mPlayer->AddComponent<PlayerScript>();
		sr = mPlayer->AddComponent<SpriteRenderer>();
		sr->SetSize(Vector2(2.0f, 2.0f));
		sr->SetTexture(Resources::Find<graphics::Texture>(L"PinkCharacter0"));

		Scene::Initialize();
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
		/*wchar_t str[50] = L"Play Scene";
		TextOut(hdc, 0, 0, str, 10);*/
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