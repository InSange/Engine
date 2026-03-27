#pragma once
#include "GameObject/NGameObject.h"
#include "Layer/NLayer.h"
#include "Component/Camera/NCamera.h"
#include "Common/NLabelled.h"

namespace NuNu
{
	class Scene : public Labelled
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
		void AddCamera(Camera* camera);
		void RemoveCamera(Camera* camera);

		const std::vector<Layer*>& GetLayers() const { return mLayers; }
		Layer* GetLayer(eLayerType type) { return mLayers[static_cast<UINT>(type)]; }
	private: 
		void createLayers();
	private:
		std::vector<Layer*> mLayers;
		std::vector<Camera*> mCameras;
	};
}


