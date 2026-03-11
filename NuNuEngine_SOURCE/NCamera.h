#pragma once
#include "NComponent.h"
namespace NuNu
{
	using namespace NuNu::math;
	class Camera : public Component
	{
	public:
		Vector2 CalculatePosition(Vector2 pos) { return pos - mDistance; }

		Camera();
		~Camera();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetTraget(GameObject* target) { mTarget = target; }

	private:
		//std::vector<GameObject*> mGameObjects;

		class GameObject* mTarget;
		Vector2 mDistance;
		Vector2 mResolution;
		Vector2 mLookPosition;
	};
}

