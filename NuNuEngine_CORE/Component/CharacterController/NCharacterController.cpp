#include "NCharacterController.h"
#include "GameObject/NGameObject.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"
#include "Collision/NCollisionManager.h"

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

		// --- 수평 이동 (WASD) — XZ 평면만 사용 (카메라 피치 Y 성분 제거) ---
		math::Vector3 pos = tr->GetPosition();
		const float speed = mMoveSpeed * dt;

		auto flatNorm = [](math::Vector3 v) -> math::Vector3
		{
			v.y = 0.0f;
			float len = v.Length();
			return (len > 0.001f) ? v / len : math::Vector3::Zero;
		};

		math::Vector3 fwd   = flatNorm(tr->Forward());
		math::Vector3 right = flatNorm(tr->Right());

		if (Input::GetKey(eKeyCode::W)) pos += fwd   * speed;
		if (Input::GetKey(eKeyCode::S)) pos -= fwd   * speed;
		if (Input::GetKey(eKeyCode::A)) pos -= right * speed;
		if (Input::GetKey(eKeyCode::D)) pos += right * speed;

		// --- 중력 & 점프 ---
		if (mbGrounded && Input::GetKeyDown(eKeyCode::SpaceBar))
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
