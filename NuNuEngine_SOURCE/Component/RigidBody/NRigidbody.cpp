#include "Component/RigidBody/NRigidbody.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"

namespace NuNu
{
	Rigidbody::Rigidbody()
		: Component(enums::eComponentType::Rigidbody)
		, mbGround(false)
		, mMass(1.0f)
		, mFriction(10.0f)
		, mForce(Vector2::Zero)
		, mVelocity(Vector2::Zero)
		, mLimitedVelocity(Vector2(200.0f, 1000.0f))
		, mGravity(Vector2(0.0f, 800.0f))
		, mAccelation(Vector2::Zero)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Initialize()
	{
	}

	void Rigidbody::Update()
	{
		// 힘 = 질량 * 가속도 -> 가속도 = 힘 / 질량
		mAccelation = mForce / mMass;

		mVelocity += mAccelation * Time::DeltaTime();

		if (mbGround)
		{	// 땅위
			Vector2 gravity = mGravity;
			gravity.Normalize();

			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{	// 공중
			mVelocity += mGravity * Time::DeltaTime();
		}

		// 최대 속도 제한
		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = mVelocity.Dot(gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;	// 좌우
		if (mLimitedVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitedVelocity.y;
		}

		if (mLimitedVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitedVelocity.x;
		}

		mVelocity = gravity + sideVelocity;

		if (mVelocity != Vector2::Zero)
		{
			// 마찰력 적용
			Vector2 friction = -mVelocity;
			friction.Normalize();
			friction = friction * mFriction * mMass * Time::DeltaTime();

			if (mVelocity.Length() <= friction.Length())
			{
				mVelocity = Vector2::Zero;
			}
			else
			{
				mVelocity += friction;
			}
		}

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		pos = pos + mVelocity * Time::DeltaTime();
		tr->SetPosition(pos);

		mForce = Vector2::One;
	}

	void Rigidbody::LateUpdate()
	{
	}

	void Rigidbody::Render()
	{
	}
}
