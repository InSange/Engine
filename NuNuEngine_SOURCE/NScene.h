#pragma once
#include "NEntity.h"
#include "NGameObject.h"
#include "NLayer.h"

namespace NuNu
{
	class Scene : public Entity
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		virtual void OnEnter();
		virtual void OnExit();

		void AddGameObject(GameObject* gameObj, enums::eLayerType type);
		Layer* GetLayer(enums::eLayerType type) { return mLayers[(UINT)type]; }
	private: 
		void createLayers();
	private:
		std::vector<Layer*> mLayers;
	};
}

