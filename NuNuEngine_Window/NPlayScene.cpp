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
#include "NAnimator.h"

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
		
		graphics::Texture* playerTexture = Resources::Find<graphics::Texture>(L"Player");
		Animator* animator = mPlayer->AddComponent<Animator>();
		animator->CreateAnimation(L"PlayerFrontMove", playerTexture
			, Vector2(0.0f, 0.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->PlayAnimation(L"PlayerFrontMove", true);

		mPlayer->GetComponent<Transform>()->SetPosition(Vector2(100.0f, 100.0f));
		mPlayer->GetComponent<Transform>()->SetScale(Vector2(2.0f, 2.0f));
		mPlayer->GetComponent<Transform>()->SetRotation(30.0f);

		graphics::Texture* mTexture = Resources::Find<graphics::Texture>(L"MapleEffect");
		GameObject* maple = object::Instantiate<GameObject>(enums::eLayerType::Particle, Vector2::Zero);
		Animator* manimator = maple->AddComponent<Animator>();
		manimator->CreateAnimation(L"MapleEffect", mTexture
			, Vector2(0.0f, 0.0f), Vector2(386.0f, 246.0f), Vector2::Zero, 8, 0.05f);
		manimator->PlayAnimation(L"MapleEffect", true);

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