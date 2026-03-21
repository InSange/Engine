#pragma once
#include "Common/CommonInclude.h"
#include "Component/Component/NComponent.h"

namespace NuNu::object
{
	void Destroy(GameObject* gameObject);
}

namespace NuNu
{
	class GameObject : public Entity
	{
	public:
		friend void object::Destroy(GameObject* obj);

		enum class eState
		{
			Created,
			Active,
			Paused,
			Dead,
			End
		};

		GameObject();
		~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

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

		eState GetState() const { return mState; }
		void SetActive(bool power)
		{
			if (power == true) mState = eState::Active;
			else mState = eState::Paused;
		}
		bool IsActive() const { return mState == eState::Active; }

		bool IsDead() const { return mState == eState::Dead; }

		void SetLayerType(eLayerType layerType) { mLayerType = layerType; }
		eLayerType GetLayerType() const { return mLayerType; }

	private:
		void initializeTransform();
		void death() { mState = eState::Dead;}

	private:
		eState mState;
		std::vector<Component*> mComponents;
		eLayerType mLayerType;
	};
}

