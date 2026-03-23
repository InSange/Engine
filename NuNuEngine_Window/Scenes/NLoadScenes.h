#pragma once
#include "Scene/NSceneManager.h"

#include "Scenes/NLoadingScene.h"
#include "Scenes/NPlayScene.h"
#include "Scenes/NHellScene.h"
#include "Scenes/NSpaceScene.h"

namespace NuNu
{
	void LoadScenes()
	{
		SceneManager::CreateScene<LoadingScene>(L"LoadingScene");
/*		SceneManager::CreateScene<SpaceScene>(L"SpaceScene");
		SceneManager::CreateScene<HellScene>(L"HellScene");
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
		SceneManager::CreateScene<ToolScene>(L"ToolScene");*/

		SceneManager::LoadScene(L"LoadingScene");
	}
}

