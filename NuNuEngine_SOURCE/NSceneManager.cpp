#include "NSceneManager.h"

namespace NuNu
{
	std::map<std::wstring, Scene*> SceneManager::mScene = {};
	Scene* SceneManager::mActiveScene = nullptr;

	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		if (mActiveScene) mActiveScene->OnExit();

		std::map<std::wstring, Scene*>::iterator it = mScene.find(name);

		if (it == mScene.end()) return nullptr;

		mActiveScene = it->second;

		if (mActiveScene)
			mActiveScene->OnEnter();

		return it->second;
	}

	void SceneManager::Initialize()
	{
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}

	void SceneManager::Render(HDC hdc)
	{
		mActiveScene->Render(hdc);
	}

}