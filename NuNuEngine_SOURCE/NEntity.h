#pragma once
#include "CommonInclude.h"

namespace NuNu
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render(HDC hdc);

		void SetName(const std::wstring& name) { mName = name; }
		std::wstring& GetName() { return mName; }

	private:
		std::wstring mName;
	};
}

