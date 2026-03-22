#include "NLoadingScene.h"
#include "../NuNuEngine_SOURCE/Renderer/NRenderer.h"
#include "../NuNuEngine_SOURCE/Scene/NSceneManager.h"
#include "../NuNuEngine_SOURCE/Resource/NResources.h"
#include "../NuNuEngine_SOURCE/Resource/Texture/NTexture.h"
#include "../../NuNuEngine_SOURCE/High Level Interface/NApplication.h"
#include "../../NuNuEngine_SOURCE/Scene/NSceneManager.h"
#include "NSpaceScene.h"
#include "NPlayScene.h"

extern NuNu::Application application;

namespace NuNu
{
	LoadingScene::LoadingScene()
		: mbLoadCompleted(false)
		, mMutualExclusion()
		, mResourcesLoadThread()
	{
	}

	LoadingScene::~LoadingScene()
	{
		delete mResourcesLoadThread;
		mResourcesLoadThread = nullptr;
	}

	void LoadingScene::Initialize()
	{
		mResourcesLoadThread = new std::thread(&LoadingScene::resourcesLoad, this, std::ref(mMutualExclusion));
	}

	void LoadingScene::Update()
	{
	}

	void LoadingScene::LateUpdate()
	{
	}

	void LoadingScene::Render()
	{
		int a = 0;

		if (mbLoadCompleted /*&& application.IsLoaded()*/)
		{
			//만약 메인쓰레드가 종료되는데 자식쓰레드가 남아있다면
			//자식쓰레드를 메인쓰레드에 편입시켜 메인쓰레드가 종료되기전까지 block
			mResourcesLoadThread->join();

			//메인쓰레드와 완전 분리 시켜 독립적인 쓰레드 운영가능
			//mResourcesLoadThread->detach();

			SceneManager::LoadScene(L"PlayScene");
		}
	}

	void LoadingScene::OnEnter()
	{
	}

	void LoadingScene::OnExit()
	{
	}

	void LoadingScene::resourcesLoad(std::mutex& m)
	{
		while (true)
		{
			if (application.IsLoaded() == true)
				break;
		}

		m.lock();
		{
			Resources::Load<graphics::Texture>(L"BG", L"../Resources/BlackHole.jpg");
			Resources::Load<graphics::Texture>(L"Hell_BG", L"../Resources/Hell.png");
			Resources::Load<graphics::Texture>(L"Space_BG", L"../Resources/Space.jpg");
			Resources::Load<graphics::Texture>(L"Logi", L"../Resources/Logi.png");
			Resources::Load<graphics::Texture>(L"TestIcon", L"../Resources/testIcon.png");
			Resources::Load<graphics::Texture>(L"tree", L"../Resources/tree.png");
			Resources::Load<graphics::Texture>(L"PinkCharacter0", L"../Resources/Character/PinkCharacter/00_PinkCharacter.png");

			Resources::Load<graphics::Texture>(L"Player", L"../Resources/Player/player.png");
			Resources::Load<graphics::Texture>(L"Demon", L"../Resources/Monster/Demon.png");
			Resources::Load<graphics::Texture>(L"MapleEffect", L"../Resources/Player/MapleEffect.png");

			Resources::Load<graphics::Texture>(L"Overworld", L"../Resources/BackGround/Overworld.png");
			Resources::Load<graphics::Texture>(L"Dungeon", L"../Resources/BackGround/dungeon_tiles.png");

			Resources::Load<graphics::Texture>(L"HPBAR", L"..\\Resources\\HPBAR.bmp");
			Resources::Load<graphics::Texture>(L"PixelMap", L"..\\Resources\\pixelMap.bmp");

			renderer::Initialize();

			SceneManager::CreateScene<SpaceScene>(L"TitleScene");
			SceneManager::CreateScene<PlayScene>(L"PlayScene");
		}
		m.unlock();

		// complete
		mbLoadCompleted = true;
	}
}