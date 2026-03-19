#include "NPlayerScript.h"
#include "NInput.h"
#include "NTransform.h"
#include "NTime.h"
#include "NGameObject.h"
#include "NDemon.h"
#include "NDemonScript.h"
#include "NObject.h"
#include "NResources.h"
#include "NRenderer.h"
#include "NCollider.h"
#include "NRigidbody.h"
#include "NUIManager.h"
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

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		COLORREF color = mPixelMap->GetPixel(pos.x, pos.y + 50);

		Rigidbody* playerRb = GetOwner()->GetComponent<Rigidbody>();
		if (color == RGB(255, 0, 0))
		{
			playerRb->SetGround(true);

			pos.y -= 1;
			tr->SetPosition(pos);
		}
		else
		{
			playerRb->SetGround(false);
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
			Demon* mDemon = object::Instantiate<Demon>(enums::eLayerType::Enemy, Vector2(0, 0));
			mDemon->SetActive(true);
			DemonScript* mDemonScript = mDemon->AddComponent<DemonScript>();

			mDemonScript->SetPlayer(GetOwner());

			graphics::Texture* DemonTexture = Resources::Find<graphics::Texture>(L"Demon");
			Animator* DemonAnimator = mDemon->AddComponent<Animator>();
			DemonAnimator->CreateAnimation(L"DemonIdle", DemonTexture
				, Vector2(0.0f, 0.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
			DemonAnimator->CreateAnimation(L"DemonLeftMove", DemonTexture
				, Vector2(0.0f, 24.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
			DemonAnimator->CreateAnimation(L"DemonRightMove", DemonTexture
				, Vector2(96.0f, 24.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
			DemonAnimator->CreateAnimation(L"DemonUpMove", DemonTexture
				, Vector2(96.0f, 96.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);
			DemonAnimator->CreateAnimation(L"DemonDownMove", DemonTexture
				, Vector2(96.0f, 120.0f), Vector2(24.0f, 24.0f), Vector2::Zero, 4, 0.2f);

			DemonAnimator->PlayAnimation(L"DemonIdle", true);

			Transform* tr = GetOwner()->GetComponent<Transform>();

			mDemon->GetComponent<Transform>()->SetPosition(tr->GetPosition());
			mDemon->GetComponent<Transform>()->SetScale(Vector2(2.0f, 2.0f));

			Vector2 mousePos = Input::GetMousePosition();
			Vector2 playerScreenPos = renderer::mainCamera->CalculatePosition(tr->GetPosition());

			Vector2 direction = mousePos - playerScreenPos;
			direction.normalize();

			mDemonScript->mDest = direction;

/*
			mState = PlayerScript::eState::Attack;
			mAnimator->PlayAnimation(L"PlayerAttack", false);

			Vector2 mousePos = Input::GetMousePosition();*/
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
		if (Input::GetKeyDown(eKeyCode::Up))
		{
			Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();
			if (rb->GetGround())
			{
				mState = PlayerScript::eState::Walk;
				mAnimator->PlayAnimation(L"PlayerUpMove", true);

				Vector2 velocity = rb->GetVelocity();
				velocity.y = -500.0f;
				rb->SetVelocity(velocity);
				rb->SetGround(false);
			}
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

		if (Input::GetKeyDown(eKeyCode::I))
		{
			UIManager::Push(eUIType::HpBar);
			//UIManager::Push(eUIType::Button);

		}

		if (Input::GetKeyDown(eKeyCode::O))
		{
			UIManager::Pop(eUIType::HpBar);

		}
	}
	void PlayerScript::move()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();

		Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

		if (Input::GetKey(eKeyCode::Right))
		{
			//pos.x += 100.0f * Time::DeltaTime();
			rb->AddForce(Vector2(100.0f, 0.0f));
		}
		if (Input::GetKey(eKeyCode::Left))
		{
			//pos.x -= 100.0f * Time::DeltaTime();
			rb->AddForce(Vector2(-100.0f, 0.0f));
		}
		if (Input::GetKeyDown(eKeyCode::Up))
		{
			if (rb->GetGround())
			{
				Vector2 velocity = rb->GetVelocity();
				velocity.y = -500.0f;
				rb->SetVelocity(velocity);
				rb->SetGround(false);
			}
		}
		if (Input::GetKey(eKeyCode::Down))
		{
			//pos.y += 100.0f * Time::DeltaTime();
			rb->AddForce(Vector2(0.0f, 100.0f));
		}

		//tr->SetPosition(pos);

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
	void PlayerScript::OnCollisionEnter(Collider* other)
	{
		int a = 0;
	}
	void PlayerScript::OnCollisionStay(Collider* other)
	{
		int a = 0;
	}
	void PlayerScript::OnCollisionExit(Collider* other)
	{
		int a = 0;
	}
}