#pragma once
#include "Component/Component/NComponent.h"
#include "Common/NMath.h"

namespace NuNu
{
	class CollisionManager;

	class CharacterController : public Component
	{
		friend class CollisionManager;
	public:
		CharacterController();
		virtual ~CharacterController() = default;

		void Initialize() override;
		void Update()     override;

		// capsule parameters (Phase 2-2 AABB 충돌 시 활용)
		float mCapsuleHeight;
		float mCapsuleRadius;

		float mMoveSpeed;
		float mRotateSpeed;
		float mGravity;
		float mJumpSpeed;

	private:
		float           mYaw;
		float           mPitch;
		math::Vector2   mLastMousePos;
		float           mVelocityY;
		bool            mbGrounded;
	};
}
