#pragma once
#include "Common/NEntity.h"

namespace NuNu
{
	using namespace enums;

	class GameObject;
	class Component : public Entity
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		[[nodiscard]] eComponentType GetType() const { return mType; }

		[[nodiscard]] GameObject* GetOwner() const { return mOwner; }
		[[noreturn]] void SetOwner(GameObject* owner) { mOwner = owner; }

	private:
		GameObject* mOwner;
		eComponentType mType;
	};
}


