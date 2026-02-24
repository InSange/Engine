#pragma once
#include "../NuNuEngine_SOURCE/NSceneManager.h"
#include "NPlayScene.h"

namespace NuNu
{
	void LoadScenes()
	{
		SceneManager::CreateScene<PlayScene>(L"PlayScene");

		SceneManager::LoadScene(L"PlayScene");
	}
}