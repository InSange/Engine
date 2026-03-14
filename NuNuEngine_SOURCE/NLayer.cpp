#include "NLayer.h"

namespace NuNu
{
	Layer::Layer()
		: mGameObjects{}
	{
	}
	Layer::~Layer()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr) continue;

			delete gameObj;
			gameObj = nullptr;
		}
	}

	void Layer::Initialize()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr) continue;
			gameObj->Initialize();
		}
	}

	void Layer::Update()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr) continue;

			GameObject::eState state = gameObj->GetState();
			if (state == GameObject::eState::Paused || state == GameObject::eState::Destroyed)
				continue;

			gameObj->Update();
		}
	}

	void Layer::LateUpdate()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr) continue;

			GameObject::eState state = gameObj->GetState();
			if (state == GameObject::eState::Paused || state == GameObject::eState::Destroyed)
				continue;

			gameObj->LateUpdate();
		}
	}

	void Layer::Render(HDC hdc)
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr) continue;

			GameObject::eState state = gameObj->GetState();
			if (state == GameObject::eState::Paused || state == GameObject::eState::Destroyed)
				continue;

			gameObj->Render(hdc);
		}
	}

	void Layer::Destroy()
	{
		for (GameObjectIter iter = mGameObjects.begin(); iter != mGameObjects.end(); )
		{
			GameObject::eState active = (*iter)->GetState();
			if (active == GameObject::eState::Destroyed)
			{
				GameObject* deathObj = (*iter);
				iter = mGameObjects.erase(iter);

				delete deathObj;
				deathObj = nullptr;

				continue;
			}

			iter++;
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr) return;
		mGameObjects.push_back(gameObject);
	}

	void Layer::EraseGameObject(GameObject* eraseGameObj)
	{
		GameObject* buffer = eraseGameObj;
		std::erase_if(mGameObjects,
			[=](GameObject* gameObj) {
				return gameObj == eraseGameObj;
			});

		delete buffer;
		buffer = nullptr;
	}

	void Layer::findDeadGameObjects(OUT std::vector<GameObject*>& gameObjects)
	{
		for (GameObject* gameObj : mGameObjects)
		{
			GameObject::eState active = gameObj->GetState();
			if (active == GameObject::eState::Destroyed)
				gameObjects.push_back(gameObj);
		}
	}

	void Layer::deleteGameObjects(std::vector<GameObject*> gameObjects)
	{
		for (GameObject* obj : gameObjects)
		{
			delete obj;
			obj = nullptr;
		}
	}

	void Layer::eraseGameObject()
	{
		std::erase_if(mGameObjects,
			[](GameObject* gameObject)
			{
				return (gameObject)->IsDead();
			});
	}
}