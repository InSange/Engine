#pragma once
#include "NScript.h"

namespace NuNu
{
	class PlayerScript : public Script
	{
	public:
		enum class eState
		{
			Idle,
			Walk,
			Attack,
		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
		
		void AttackEnd();

	private:
		void idle();
		void move();
		void attack();
	private:
		float animTime;
		eState mState;
		class Animator* mAnimator;

		
		/*void(*StartEvent)();
		void(*CompeleteEvent)();
		void(*EndEvent)();*/
	};
}

