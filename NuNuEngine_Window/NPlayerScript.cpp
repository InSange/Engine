#include "NPlayerScript.h"
#include "NInput.h"
#include "NTransform.h"
#include "NTime.h"
#include "NGameObject.h"
#include "../NuNuEngine_SOURCE/NAnimator.h"

namespace NuNu
{
	PlayerScript::PlayerScript()
		: mState(PlayerScript::eState::Idle)
		, mAnimator(nullptr)
		, animTime(0.0f)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
	}
	void PlayerScript::Update()
	{
		if (mAnimator == nullptr) mAnimator = GetOwner()->GetComponent<Animator>();
		switch (mState)
		{
		case NuNu::PlayerScript::eState::Idle:
			idle();
			break;
		case NuNu::PlayerScript::eState::Walk:
			move();
			break;
		case NuNu::PlayerScript::eState::Attack:
			attack();
			break;
		default:
			break;
		}
	}
	void PlayerScript::LateUpdate()
	{
	}
	void PlayerScript::Render(HDC hdc)
	{
	}
	void PlayerScript::idle()
	{
		if (Input::GetKey(eKeyCode::LButton))
		{
			mState = PlayerScript::eState::Attack;
			mAnimator->PlayAnimation(L"PlayerAttack", false);

			Vector2 mousePos = Input::GetMousePosition();
		}

		if (Input::GetKey(eKeyCode::Right))
		{
			mState = PlayerScript::eState::Walk;
			mAnimator->PlayAnimation(L"PlayerRightMove", true);
		}
		if (Input::GetKey(eKeyCode::Left))
		{
			mState = PlayerScript::eState::Walk;
			mAnimator->PlayAnimation(L"PlayerRightMove", true);
		}
		if (Input::GetKey(eKeyCode::Up))
		{
			mState = PlayerScript::eState::Walk;
			mAnimator->PlayAnimation(L"PlayerUpMove", true);
		}
		if (Input::GetKey(eKeyCode::Down))
		{
			mState = PlayerScript::eState::Walk;
			mAnimator->PlayAnimation(L"PlayerUpMove", true);
		}
		/*if (Input::GetKey(eKeyCode::SpaceBar))
		{
			animTime = 0.0f;
			mState = PlayerScript::eState::Attack;
			mAnimator->PlayAnimation(L"PlayerAttack", false);
		}*/
	}
	void PlayerScript::move()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();


		if (Input::GetKey(eKeyCode::Right))
		{
			pos.x += 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(eKeyCode::Left))
		{
			pos.x -= 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(eKeyCode::Up))
		{
			pos.y -= 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(eKeyCode::Down))
		{
			pos.y += 100.0f * Time::DeltaTime();
		}

		tr->SetPosition(pos);

		if (Input::GetKeyUp(eKeyCode::Right) || Input::GetKeyUp(eKeyCode::Left)
			|| Input::GetKeyUp(eKeyCode::Up) || Input::GetKeyUp(eKeyCode::Down))
		{
			mState = PlayerScript::eState::Idle;
			mAnimator->PlayAnimation(L"PlayerIdle", true);
		}
	}

	void PlayerScript::attack()
	{
		/*if (mAnimator->IsCompleteAnimation())
		{
			mState = PlayerScript::eState::Idle;
			mAnimator->PlayAnimation(L"PlayerIdle", true);
		}*/
	}

	void PlayerScript::AttackEnd()
	{
		if (mAnimator->IsCompleteAnimation())
		{
			mState = PlayerScript::eState::Idle;
			mAnimator->PlayAnimation(L"PlayerIdle", true);
		}
	}
}