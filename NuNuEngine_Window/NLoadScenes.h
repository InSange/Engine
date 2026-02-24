#pragma once
#include "../NuNuEngine_SOURCE/NSceneManager.h"
#include "NPlayScene.h"
#include "NHellScene.h"

namespace NuNu
{
	void LoadScenes()
	{
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
		SceneManager::CreateScene<HellScene>(L"HellScene");

		SceneManager::LoadScene(L"HellScene");
	}
}