#pragma once
#include "NEntity.h"
#include "CommonInclude.h"
#include "NGameObject.h"

namespace NuNu
{
	class Layer : public Entity
	{
	public:
		Layer();
		~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);
		virtual void Destroy();

		void AddGameObject(GameObject* gameObject);
		void EraseGameObject(GameObject* eraseGameObj);
		//void SetType(enums::eLayerType type) { mType = type; }

		const std::vector<GameObject*> GetGameObjects() { return mGameObjects; }

	private:
		void findDeadGameObjects(OUT std::vector<GameObject*>& gameObjects);
		void deleteGameObjects(std::vector<GameObject*> deleteObjs);
		void eraseDeadGameObject();

	private:
		//enums::eLayerType mType;
		std::vector<GameObject*> mGameObjects;
	};


	using GameObjectIter = std::vector<GameObject*>::iterator;
//	typedef std::vector<GameObject*>::iterator GameObjectIter;
}

