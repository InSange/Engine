#pragma once
#include "Common/NEntity.h"
#include "GameObject/NGameObject.h"
#include "Layer/NLayer.h"

namespace NuNu
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void EndOfFrame();

		virtual void OnEnter();
		virtual void OnExit();

		void AddGameObject(GameObject* gameObj, eLayerType type);
		void EraseGameObject(GameObject* gameObj);
		Layer* GetLayer(eLayerType type) { return mLayers[static_cast<UINT>(type)]; }
	private: 
		void createLayers();
	private:
		std::vector<Layer*> mLayers;
	};
}


