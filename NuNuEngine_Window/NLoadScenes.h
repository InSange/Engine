#pragma once
#include "../NuNuEngine_SOURCE/NSceneManager.h"
#include "NPlayScene.h"
#include "NHellScene.h"
#include "NSpaceScene.h"

namespace NuNu
{


	void LoadScenes()
	{
		SceneManager::CreateScene<SpaceScene>(L"SpaceScene");
		SceneManager::CreateScene<HellScene>(L"HellScene");
		SceneManager::CreateScene<PlayScene>(L"PlayScene");

		SceneManager::LoadScene(L"PlayScene");
	}
}