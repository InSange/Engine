#include "Scripts/NDemonScript.h"
#include "Helpers/NInput.h"
#include "Component/Transform/NTransform.h"
#include "Helpers/NTime.h"
#include "GameObject/NGameObject.h"
#include "Component/Animator/NAnimator.h"
#include "Object/NObject.h"
#include "Component/Collider/NCollider.h"

namespace NuNu
{
	DemonScript::DemonScript()
		: mState(DemonScript::eState::Idle)
		, mAnimator(nullptr)
		, mTime(0.0f)
		, mDeathTime(0.0f)
		, mDest(Vector2::Zero)
		, mRadian(0.0f)
	{
	}
	DemonScript::~DemonScript()
	{
	}
	void DemonScript::Initialize()
	{
	}
	void DemonScript::Update()
	{
		mDeathTime += Time::DeltaTime();
		if (mDeathTime > 3.0f)
		{
			//object::Destroy(GetOwner());
		}

		mTime += Time::DeltaTime();
		if (mAnimator == nullptr) mAnimator = GetOwner()->GetComponent<Animator>();
		switch (mState)
		{
		case NuNu::DemonScript::eState::Idle:
			idle();
			break;
		case NuNu::DemonScript::eState::Walk:
			move();
			break;
		case NuNu::DemonScript::eState::Attack:
			attack();
			break;
		default:
			break;
		}
	}
	void DemonScript::LateUpdate()
	{
	}
	void DemonScript::Render()
	{
	}
	void DemonScript::OnCollisionEnter(Collider* other)
	{
	}
	void DemonScript::OnCollisionStay(Collider* other)
	{
	}
	void DemonScript::OnCollisionExit(Collider* other)
	{
	}
	void DemonScript::idle()
	{
/*		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();


		float rotDegree = Vector2::Dot(mDest, Vector2::Right);
		rotDegree = acosf(rotDegree); 
		rotDegree = ConvertDegree(rotDegree); 

		if (mDest.y < 0.0f)
		{
			rotDegree = 360.0f - rotDegree;
		}

		tr->SetRotation(rotDegree);

		pos.x += mDest.x * 100.0f * Time::DeltaTime();
		pos.y += mDest.y * 100.0f * Time::DeltaTime();
		tr->SetPosition(pos);*/


		/*if (mTime > 3.0f)
		{
			mState = DemonScript::eState::Walk;
			mDir = (eDirection)(rand() % 4);
			playWalkAnimationByDirection(mDir);
			mTime = 0.0f;
		}*/
	}
	void DemonScript::move()
	{
		if (mTime > 2.0f)
		{
			mState = DemonScript::eState::Idle;
			mAnimator->PlayAnimation(L"DemonIdle", true);
			mTime = 0.0f;
		}

		Transform* tr = GetOwner()->GetComponent<Transform>();

		translate(tr);
	}

	void DemonScript::attack()
	{
		if (mTime >= 1.0f)
		{
			mState = DemonScript::eState::Idle;
			mAnimator->PlayAnimation(L"DemonIdle", true);
		}
	}
	void DemonScript::playWalkAnimationByDirection(eDirection dir)
	{
		switch (dir)
		{
		case NuNu::DemonScript::eDirection::Left:
			mAnimator->PlayAnimation(L"DemonLeftMove", true);
			break;
		case NuNu::DemonScript::eDirection::Right:
			mAnimator->PlayAnimation(L"DemonRightMove", true);
			break;
		case NuNu::DemonScript::eDirection::Up:
			mAnimator->PlayAnimation(L"DemonUpMove", true);
			break;
		case NuNu::DemonScript::eDirection::Down:
			mAnimator->PlayAnimation(L"DemonDownMove", true);
			break;
		default:
			assert(false);
			break;
		}
	}
	void DemonScript::translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();
		switch (mDir)
		{
		case NuNu::DemonScript::eDirection::Left:
			pos.x -= 100.0f * Time::DeltaTime();
			break;
		case NuNu::DemonScript::eDirection::Right:
			pos.x += 100.0f * Time::DeltaTime();
			break;
		case NuNu::DemonScript::eDirection::Up:
			pos.y -= 100.0f * Time::DeltaTime();
			break;
		case NuNu::DemonScript::eDirection::Down:
			pos.y += 100.0f * Time::DeltaTime();
			break;
		default:
			assert(false);
			break;
		}

		tr->SetPosition(pos);
	}
}


