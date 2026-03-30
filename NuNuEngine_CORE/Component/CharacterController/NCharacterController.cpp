#include "NCharacterController.h"
#include "GameObject/NGameObject.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"

namespace NuNu
{
	CharacterController::CharacterController()
		: Component(enums::eComponentType::CharacterController)
		, mCapsuleHeight(1.8f)
		, mCapsuleRadius(0.4f)
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

		// --- 카메라 회전 ---
		math::Vector2 curPos = Input::GetMousePosition();
		const bool curValid  = curPos.x >= 0.0f && curPos.y >= 0.0f;
		const bool lastValid = mLastMousePos.x >= 0.0f && mLastMousePos.y >= 0.0f;

		if (curValid && lastValid)
		{
			math::Vector2 delta = curPos - mLastMousePos;

			mYaw   += delta.x * mRotateSpeed;
			mPitch += delta.y * mRotateSpeed;
			if (mPitch >  89.0f) mPitch =  89.0f;
			if (mPitch < -89.0f) mPitch = -89.0f;

			tr->SetRotation(mPitch, mYaw, 0.0f);
		}
		mLastMousePos = curPos;

		// --- 수평 이동 (WASD) ---
		math::Vector3 pos = tr->GetPosition();
		const float speed = mMoveSpeed * dt;

		if (Input::GetKey(eKeyCode::W)) pos += tr->Forward() * speed;
		if (Input::GetKey(eKeyCode::S)) pos -= tr->Forward() * speed;
		if (Input::GetKey(eKeyCode::A)) pos -= tr->Right()   * speed;
		if (Input::GetKey(eKeyCode::D)) pos += tr->Right()   * speed;

		// --- 중력 & 점프 ---
		if (mbGrounded && Input::GetKeyDown(eKeyCode::SpaceBar))
		{
			mVelocityY = mJumpSpeed;
			mbGrounded = false;
		}

		mVelocityY += mGravity * dt;
		pos.y      += mVelocityY * dt;

		// 바닥 감지 (y <= 0 하드코딩, Phase 2-2에서 AABB로 교체)
		if (pos.y <= 0.0f)
		{
			pos.y      = 0.0f;
			mVelocityY = 0.0f;
			mbGrounded = true;
		}

		tr->SetPosition(pos);
	}
}
