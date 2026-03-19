#pragma once
#include "Component/Script/NScript.h"
#include "Component/Transform/NTransform.h"

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

		void SetPlayer(class GameObject* player) { mPlayer = player; }

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		Vector2 mDest;

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

		GameObject* mPlayer;
		float mRadian;
		
	};
}


