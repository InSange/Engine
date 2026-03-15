#pragma once
#include "../NuNuEngine_SOURCE/NSceneManager.h"
#include "NPlayScene.h"
#include "NHellScene.h"
#include "NSpaceScene.h"
#include "NToolScene.h"

namespace NuNu
{


	void LoadScenes()
	{/*
		SceneManager::CreateScene<SpaceScene>(L"SpaceScene");
		SceneManager::CreateScene<HellScene>(L"HellScene");*/
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
		SceneManager::CreateScene<ToolScene>(L"ToolScene");

		SceneManager::LoadScene(L"PlayScene");
	}
}