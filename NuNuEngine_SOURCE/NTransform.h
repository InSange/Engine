#pragma once
#include "NComponent.h"

namespace NuNu
{
	struct Position
	{
		int X;
		int Y;
	};

	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetPosition(int x, int y) { mX = x; mY = y; }
		
		int GetX() { return mX; }
		int GetY() { return mY; }

	private:
		int mX;
		int mY;
	};
}

