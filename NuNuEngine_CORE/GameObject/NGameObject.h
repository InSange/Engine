#pragma once
#include "Common/CommonInclude.h"
#include "Component/Component/NComponent.h"

namespace NuNu::object
{
	extern void Destroy(GameObject* gameObject);
}

namespace NuNu
{
	class GameObject : public Labelled
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
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(const Matrix& view, const Matrix& projection);

		template<typename T>
		T* AddComponent()
		{
			T* comp = new T();
			comp->Initialize();
			comp->SetOwner(this);

			mComponents[static_cast<UINT>(comp->GetType())] = comp;

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

		const std::vector<Component*>& GetComponents() const { return mComponents; }

		eState GetState() const { return State; }
		void SetActive(bool power)
		{
			if (power == true) State = eState::Active;
			else State = eState::Paused;
		}

		bool IsActive() const { return State == eState::Active; }
		bool IsDead() const { return State == eState::Destroyed; }
		eLayerType GetLayerType() const { return mLayerType; }
		void SetLayerType(const eLayerType layerType) { mLayerType = layerType; }

	private:
		void initializeTransform();
		void death() { State = eState::Destroyed; }

	private:
		eState State;
		std::vector<Component*> mComponents;
		eLayerType mLayerType;
	};
}

