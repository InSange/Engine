#pragma once
#include "CommonInclude.h"
#include "NComponent.h"

namespace NuNu::object
{
	void Destroy(GameObject* gameObject);
}

namespace NuNu
{
	class GameObject// : public Labelled
	{
	public:
		friend void object::Destroy(GameObject* obj);

		enum class eState
		{
			Created,
			Active,
			Paused,
			Destroyed,
			End
		};

		GameObject();
		~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		template<typename T>
		T* AddComponent()
		{
			T* comp = new T();
			comp->Initialize();
			comp->SetOwner(this);

			mComponents[(UINT)comp->GetType()] = comp;

			return comp;
		}

		template<typename T>
		T* GetComponent()
		{
			T* component = nullptr;
			for (Component* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (component) break;
			}

			return component;
		}

		eState GetState() { return mState; }
		void SetActive(bool power)
		{
			if (power == true) mState = eState::Active;
			else mState = eState::Paused;
		}
		bool IsActive() { return mState == eState::Active; }
		
		void Death() { mState = eState::Destroyed; }
		bool IsDead() { return mState == eState::Destroyed; }

		void SetLayerType(eLayerType layerType) { mLayerType = layerType; }
		eLayerType GetLayerType() { return mLayerType; }

	private:
		void initializeTransform();

	private:
		eState mState;
		std::vector<Component*> mComponents;
		eLayerType mLayerType;
	};
}
