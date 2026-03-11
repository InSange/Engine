#pragma once
#include "NScript.h"
#include "NTransform.h"

namespace NuNu
{
	class DemonScript : public Script
	{
	public:
		enum class eState
		{
			Idle,
			Walk,
			Attack,
		};

		enum class eDirection
		{
			Left,
			Right,
			Up,
			Down,
			End
		};

		DemonScript();
		~DemonScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:
		void idle();
		void move();
		void attack();
		void playWalkAnimationByDirection(eDirection dir);
		void translate(Transform* tr);

	private:
		eState mState;
		class Animator* mAnimator;
		eDirection mDir;
		float mTime;
		float mDeathTime;
	};
}

