#pragma once
#include "NScene.h"

namespace NuNu
{
	class SceneManager
	{
	public:
		template <typename T>
		static Scene* CreateScene(const std::wstring& name)
		{
			T* scene = new T();
			scene->SetName(name);
			scene->Initialize();

			mScene.insert(std::make_pair(name, scene));
			return scene;
		}

		static Scene* LoadScene(const std::wstring& name)
		{
			if(mActiveScene) mActiveScene->OnExit();

			std::map<std::wstring, Scene*>::iterator it = mScene.find(name);

			if (it == mScene.end()) return nullptr;

			mActiveScene = it->second;

			if (mActiveScene) 
				mActiveScene->OnEnter();

			return it->second;
		}

		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hdc);

	private:
		//static std::vector<Scene*> mScene;
		static std::map<std::wstring, Scene*> mScene;
		static Scene* mActiveScene;
	};
}
