#include "NCharacterController.h"
#include "GameObject/NGameObject.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"
#include "Collision/NCollisionManager.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NKarmaComponent.h"

namespace NuNu
{
	CharacterController::CharacterController()
		: Component(enums::eComponentType::CharacterController)
		, mCapsuleHeight(1.8f)
		, mCapsuleRadius(0.4f)
		, mEyeHeight(1.7f)
		, mMoveSpeed(5.0f)
		, mRotateSpeed(0.2f)
		, mGravity(-20.0f)
		, mJumpSpeed(8.0f)
		, mYaw(0.0f)
		, mPitch(0.0f)
		, mLastMousePos(math::Vector2::Zero)
		, mVelocityY(0.0f)
		, mbGrounded(false)
	{
	}

	void CharacterController::Initialize()
	{
		Component::Initialize();
	}

	void CharacterController::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		const float dt = Time::DeltaTime();

		CurseComponent*  curse = GetOwner()->GetComponent<CurseComponent>();
		KarmaComponent*  karma = GetOwner()->GetComponent<KarmaComponent>();

		// 스턴 상태이면 이동/조작 전부 스킵
		if (karma && karma->IsStunned())
			return;

		// --- 카메라 회전 ---
		math::Vector2 curPos = Input::GetMousePosition();
		const bool curValid  = curPos.x >= 0.0f && curPos.y >= 0.0f;
		const bool lastValid = mLastMousePos.x >= 0.0f && mLastMousePos.y >= 0.0f;

		if (curValid && lastValid)
		{
			math::Vector2 delta = curPos - mLastMousePos;

			const float pitchSign = (curse && curse->IsVisionInverted()) ? -1.0f : 1.0f;
			mYaw   += delta.x * mRotateSpeed;
			mPitch += delta.y * mRotateSpeed * pitchSign;
			if (mPitch >  89.0f) mPitch =  89.0f;
			if (mPitch < -89.0f) mPitch = -89.0f;

			tr->SetRotation(mPitch, mYaw, 0.0f);
		}
		mLastMousePos = curPos;

		// --- 수평 이동 (WASD) — XZ 평면만 사용 (카메라 피치 Y 성분 제거) ---
		math::Vector3 pos = tr->GetPosition();
		float speedMult = (curse) ? curse->GetMoveSpeedMultiplier() : 1.0f;
		if (karma && karma->IsSpeedBoosted()) speedMult *= 1.5f;
		const float speed = mMoveSpeed * speedMult * dt;
		const float moveSign = (curse && curse->IsControlInverted()) ? -1.0f : 1.0f;

		auto flatNorm = [](math::Vector3 v) -> math::Vector3
		{
			v.y = 0.0f;
			float len = v.Length();
			return (len > 0.001f) ? v / len : math::Vector3::Zero;
		};

		math::Vector3 fwd   = flatNorm(tr->Forward());
		math::Vector3 right = flatNorm(tr->Right());

		if (Input::GetKey(eKeyCode::W)) pos += fwd   * speed * moveSign;
		if (Input::GetKey(eKeyCode::S)) pos -= fwd   * speed * moveSign;
		if (Input::GetKey(eKeyCode::A)) pos -= right * speed * moveSign;
		if (Input::GetKey(eKeyCode::D)) pos += right * speed * moveSign;

		// --- 중력 & 점프 ---
		const bool forceJump = (curse && curse->ConsumeForceJump());
		if (mbGrounded && (Input::GetKeyDown(eKeyCode::SpaceBar) || forceJump))
		{
			mVelocityY = mJumpSpeed;
			mbGrounded = false;
		}

		mVelocityY += mGravity * dt;
		pos.y      += mVelocityY * dt;

		tr->SetPosition(pos);
		CollisionManager::Resolve3D(this);
	}
}
