#pragma once
#include "Common/NLabelled.h"

namespace NuNu
{
	using namespace enums;

	class GameObject;
	class Component : public Labelled
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(const Matrix& view, const Matrix& projection);

		eComponentType GetType() const { return mType; }

		// 직렬화 키 — 직렬화 가능 컴포넌트는 고유 문자열 반환, 기본값 nullptr = 직렬화 제외
		virtual const char* GetComponentKey() const { return nullptr; }

		GameObject* GetOwner() const { return mOwner; }
		void SetOwner(GameObject* owner) { mOwner = owner; }

	private:
		GameObject* mOwner;
		eComponentType mType;
	};
}


