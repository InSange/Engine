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

		void AddGameObject(GameObject* gameObject);
		void SetType(eLayerType type) { mType = type; }
	private:
		eLayerType mType;
		std::vector<GameObject*> mGameObjects;
	};
}

