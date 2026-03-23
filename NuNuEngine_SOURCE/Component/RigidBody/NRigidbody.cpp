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

	}

	void Rigidbody::LateUpdate()
	{
	}

	void Rigidbody::Render()
	{
	}
}
