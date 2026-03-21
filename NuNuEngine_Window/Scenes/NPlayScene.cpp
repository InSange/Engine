#include "Scenes/NPlayScene.h"
#include "GameObject/NGameObject.h"
#include "Contents/NPlayer.h"
#include "UI/NUIManager.h"
#include "Contents/NDemon.h"
#include "Component/Transform/NTransform.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "Helpers/NInput.h"
#include "Scenes/NSpaceScene.h"
#include "Scene/NSceneManager.h"
#include "Common/NObject.h"
#include "Resource/Texture/NTexture.h"
#include "Resource/NResources.h"
#include "Scripts/NPlayerScript.h"
#include "Scripts/NDemonScript.h"
#include "Component/Camera/NCamera.h"
#include "Renderer/NRenderer.h"
#include "Component/Animation/NAnimator.h"
#include "Component/Collider/NBoxCollider2D.h"
#include "Component/Collider/NCircleCollider2D.h"
#include "NCollisionManager.h"
#include "Contents/NTile.h"
#include "Component/TileMapRenderer/NTilemapRenderer.h"
#include "Component/RigidBody/NRigidbody.h"
#include "Contents/NFloor.h"
#include "Scripts/NFloorScript.h"
#include "Resource/Audio/NAudioClip.h"
#include "Component/Audio/NAudioListener.h"
#include "Component/Audio/NAudioSource.h"

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
/*		FILE* pFile = nullptr;
		_wfopen_s(&pFile, L"..\\Resources\\Tile\\TileData.tile", L"rb");

		while (true)
		{
			int idxX = 0;
			int idxY = 0;

			int posX = 0;
			int posY = 0;

			if (fread(&idxX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&idxY, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posY, sizeof(int), 1, pFile) == NULL)
				break;

			Tile* tile = object::Instantiate<Tile>(eLayerType::Tile, Vector2(posX, posY));
			TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();
			tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));
			tmr->SetIndex(Vector2(idxX, idxY));
		}

		fclose(pFile);*/

/*		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);*/
/*
		Scene::Initialize();
		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector2(812.0f, 470.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComp;

		//camera->AddComponent<PlayerScript>();

		GameObject* bgObj = object::Instantiate<GameObject>(enums::eLayerType::BackGround, Vector2::Zero);
		SpriteRenderer* sr = bgObj->AddComponent<SpriteRenderer>();
		sr->SetTexture(Resources::Find<graphics::Texture>(L"BG"));

		bgObj->SetActive(false);*/

		/*		for (size_t i = 0; i < 5; i++)
				{
					GameObject* obj = object::Instantiate<GameObject>(enums::eLayerType::UI, Vector2(rand() % 1600, rand() % 900));

					sr = obj->AddComponent<SpriteRenderer>();
					sr->SetTexture(Resources::Find<graphics::Texture>(L"TestIcon"));
				}*/

/*		mPlayer = object::Instantiate<Player>(enums::eLayerType::Player, Vector2(0, 0));
		object::DontDestroyOnLoad(mPlayer);

		PlayerScript* playerScript = mPlayer->AddComponent<PlayerScript>();
		mPlayer->AddComponent<Rigidbody>();
		mPlayer->AddComponent<AudioListener>();
		BoxCollider2D* collider = mPlayer->AddComponent<BoxCollider2D>();
		//CircleCollider2D* collider = mPlayer->AddComponent<CircleCollider2D>();
		collider->SetOffset(Vector2(-50.0f, -50.0f));*/

		//cameraComp->SetTraget(mPlayer);

/*		graphics::Texture* playerTexture = Resources::Find<graphics::Texture>(L"Player");
		Animator* animator = mPlayer->AddComponent<Animator>();
		animator->CreateAnimation(L"PlayerIdle", playerTexture
			, Vector2(0.0f, 0.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->CreateAnimation(L"PlayerDownMove", playerTexture
			, Vector2(0.0f, 144.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->CreateAnimation(L"PlayerLeftMove", playerTexture
			, Vector2(0.0f, 192.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->CreateAnimation(L"PlayerRightMove", playerTexture
			, Vector2(0.0f, 192.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->CreateAnimation(L"PlayerUpMove", playerTexture
			, Vector2(0.0f, 240.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 6, 0.2f);
		animator->CreateAnimation(L"PlayerAttack", playerTexture
			, Vector2(0.0f, 288.0f), Vector2(48.0f, 48.0f), Vector2::Zero, 4, 0.2f);

		animator->GetCompleteEvent(L"PlayerAttack") = std::bind(&PlayerScript::AttackEnd, playerScript);
		animator->PlayAnimation(L"PlayerIdle", true);

		mPlayer->GetComponent<Transform>()->SetPosition(Vector2(500.0f, 500.0f));
		mPlayer->GetComponent<Transform>()->SetScale(Vector2(2.0f, 2.0f));*/
		//mPlayer->GetComponent<Transform>()->SetRotation(30.0f);

		/*graphics::Texture* mTexture = Resources::Find<graphics::Texture>(L"MapleEffect");
		GameObject* maple = object::Instantiate<GameObject>(enums::eLayerType::Particle, Vector2::Zero);
		Animator* manimator = maple->AddComponent<Animator>();
		manimator->CreateAnimation(L"MapleEffect", mTexture
			, Vector2(0.0f, 0.0f), Vector2(386.0f, 246.0f), Vector2::Zero, 8, 0.05f);
		manimator->PlayAnimation(L"MapleEffect", true);*/

/*		Floor* floor = object::Instantiate<Floor>(eLayerType::Floor, Vector2(0.0f, 0.0f));
		floor->SetName(L"Floor");
		FloorScript* floorScript = floor->AddComponent<FloorScript>();
		AudioSource* as = floor->AddComponent<AudioSource>();
		SpriteRenderer* floorSR = floor->AddComponent<SpriteRenderer>();
		floorSR->SetTexture(Resources::Find<graphics::Texture>(L"PixelMap"));
		playerScript->SetPixelMapTexture(Resources::Find<graphics::Texture>(L"PixelMap"));*/

		/*BoxCollider2D* floorCol =  floor->AddComponent<BoxCollider2D>();

		floorCol->SetSize(Vector2(3.0f, 1.0f));*/

/*		Demon* mDemon = object::Instantiate<Demon>(enums::eLayerType::Enemy, Vector2(0, 0));
		mDemon->SetActive(true);
		mDemon->AddComponent<DemonScript>();

		graphics::Texture* DemonTexture = Resources::Find<graphics::Texture>(L"Demon");
		Animator* DemonAnimator = mDemon->AddComponent<Animator>();

		//BoxCollider2D* boxDemonCollider = mDemon->AddComponent<BoxCollider2D>();
		CircleCollider2D* boxDemonCollider = mDemon->AddComponent<CircleCollider2D>();
		boxDemonCollider->SetOffset(Vector2(-50.0f, -50.0f));

		DemonAnimator->CreateAnimation(L"DemonIdle", DemonTexture
			, Vector2(0.0f, 0.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
		DemonAnimator->CreateAnimation(L"DemonLeftMove", DemonTexture
			, Vector2(0.0f, 24.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
		DemonAnimator->CreateAnimation(L"DemonRightMove", DemonTexture
			, Vector2(96.0f, 24.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
		DemonAnimator->CreateAnimation(L"DemonUpMove", DemonTexture
			, Vector2(96.0f, 96.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
		DemonAnimator->CreateAnimation(L"DemonDownMove", DemonTexture
			, Vector2(96.0f, 120.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);

		DemonAnimator->PlayAnimation(L"DemonIdle", true);

		mDemon->GetComponent<Transform>()->SetPosition(Vector2(500.0f, 500.0f));
		mDemon->GetComponent<Transform>()->SetScale(Vector2(2.0f, 2.0f));
		mDemon->GetComponent<Transform>()->SetRotation(30.0f);*/

/*		GameObject* obj = object::Instantiate<GameObject>(enums::eLayerType::UI, Vector2(rand() % 1600, rand() % 900));
		obj->GetComponent<Transform>()->SetPosition(Vector2(100.0f, 100.0f));

		Animator* objAnim = obj->AddComponent<Animator>();
		objAnim->CreateAnimationByFoler(L"Attack", L"../Resources/PlayerAttack", Vector2::Zero, 0.1f);
		objAnim->PlayAnimation(L"Attack", true);*/

		/*		SpriteRenderer* asr = obj->AddComponent<SpriteRenderer>();
				asr->SetTexture(Resources::Find<graphics::Texture>(L"Attack"));*/
/*
		AudioClip* ac = Resources::Load<AudioClip>(L"BGSound", L"..\\Resources\\Sound\\smw_bonus_game_end.wav");
		as->SetClip(ac);*/
		//as->Play();

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
		Scene::OnEnter();

		// CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);
		// CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Floor, true);

		// UIManager::Push(eUIType::Button);
	}
	void PlayScene::OnExit()
	{
		/*		Transform* tr = bg->GetComponent<Transform>();
				tr->SetPosition(Vector2(0, 0));*/

		// UIManager::Pop(eUIType::Button);


		Scene::OnExit();
	}
}

