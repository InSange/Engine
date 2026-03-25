#pragma once
#include "Component/Component/NComponent.h"

namespace NuNu
{
	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		Vector2 GetVelocity() const { return mVelocity; }
		void SetMass(float mass) { mMass = mass; }
		void AddForce(Vector2 force) { mForce = force; }

		void SetGround(bool ground) { mbGround = ground; }
		bool GetGround() { return mbGround; }
		void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

	private:
		bool mbGround;
		float mMass;
		float mFriction;

		Vector2 mForce;
		Vector2 mAccelation;
		Vector2 mVelocity;
		Vector2 mLimitedVelocity;
		Vector2 mGravity;
	};
}


