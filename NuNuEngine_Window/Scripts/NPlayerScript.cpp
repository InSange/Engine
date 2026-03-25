#include "Scripts/NPlayerScript.h"
#include "Helpers/NInput.h"
#include "Component/Transform/NTransform.h"
#include "Helpers/NTime.h"
#include "GameObject/NGameObject.h"
#include "Contents/NDemon.h"
#include "Object/NObject.h"
#include "Resource/NResources.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Component/Collider/NCollider.h"
#include "Component/RigidBody/NRigidbody.h"
#include "UI/NUIManager.h"
#include "Component/Animator/NAnimator.h"
#include "Component/SpriteRenderer/NSpriteRenderer.h"

namespace NuNu
{
	PlayerScript::PlayerScript()
		: mState(eState::Idle)
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
		static Vector3 postions = Vector3(-1, 1, 0);

		if (Input::GetKeyDown(eKeyCode::N))
		{
			mProjTile = object::Instantiate<ProjectTile>(eLayerType::Player, postions);
			SpriteRenderer* sr = mProjTile->AddComponent<SpriteRenderer>();
			sr->SetSprite(Resources::Find<graphics::Texture>(L"Player"));

			postions.x += 1.0f;
		}

		if (Input::GetKeyDown(eKeyCode::D))
		{
			object::Destroy(mProjTile);
			mProjTile = nullptr;
		}
	}
	void PlayerScript::LateUpdate()
	{
	}
	void PlayerScript::Render(const Matrix& view, const Matrix& projection)
	{
	}
	void PlayerScript::idle()
	{

	}
	void PlayerScript::move()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

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
			mState = eState::Idle;
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
/*		if (mAnimator->IsCompleteAnimation())
		{
			mState = PlayerScript::eState::Idle;
			mAnimator->PlayAnimation(L"PlayerIdle", true);
		}*/
	}
	void PlayerScript::OnCollisionEnter(Collider* other)
	{
	}
	void PlayerScript::OnCollisionStay(Collider* other)
	{
	}
	void PlayerScript::OnCollisionExit(Collider* other)
	{
	}
}


