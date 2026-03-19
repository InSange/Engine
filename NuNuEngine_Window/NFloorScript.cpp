#include "NFloorScript.h"
#include "NInput.h"
#include "NTransform.h"
#include "NTime.h"
#include "NGameObject.h"
#include "NAnimator.h"
#include "NObject.h"
#include "NRigidbody.h"
#include "NCollider.h"
#include "NAudioSource.h"

namespace NuNu
{
	FloorScript::FloorScript()
	{
	}

	FloorScript::~FloorScript()
	{
	}

	void FloorScript::Initialize()
	{
	}

	void FloorScript::Update()
	{
	}

	void FloorScript::LateUpdate()
	{
	}

	void FloorScript::Render(HDC hdc)
	{
	}

	void FloorScript::OnCollisionEnter(Collider* other)
	{
		Rigidbody* otherRb = other->GetOwner()->GetComponent<Rigidbody>();
		if (otherRb == nullptr) return;

		// 만약 위로 올라가는 중(점프 등)이라면 바닥에 안착시키지 않음
		if (otherRb->GetVelocity().y < 0.0f) return;

		Transform* otherTr = other->GetOwner()->GetComponent<Transform>();
		Collider* otherCol = other;

		Rigidbody* floorRb = this->GetOwner()->GetComponent<Rigidbody>();
		Transform* floorTr = this->GetOwner()->GetComponent<Transform>();
		Collider* floorCol = this->GetOwner()->GetComponent<Collider>();

		Vector2 otherPos = otherTr->GetPosition() + otherCol->GetOffset();
		Vector2 floorPos = floorTr->GetPosition() + floorCol->GetOffset();

		float otherBottom = otherPos.y + (otherCol->GetSize().y * 100.0f / 2.0f);
		float floorTop = floorPos.y - (floorCol->GetSize().y * 100.0f / 2.0f);

		if (otherBottom > floorTop && otherPos.y < floorPos.y)
		{
			Vector2 otherTrPos = otherTr->GetPosition();
			otherTrPos.y -= (otherBottom - floorTop) - 0.01f;
			otherTr->SetPosition(otherTrPos);

			AudioSource* as = GetOwner()->GetComponent<AudioSource>();
			//as->SetClip();
			as->SetLoop(true);
			as->Play();

			otherRb->SetGround(true);
		}
	}

	void FloorScript::OnCollisionStay(Collider* other)
	{
		Rigidbody* otherRb = other->GetOwner()->GetComponent<Rigidbody>();
		if (otherRb != nullptr)
		{
			// 점프 뛰어서 올라가는 중이면 무시
			if (otherRb->GetVelocity().y < 0.0f) return;

			// 땅에 닿았다고 처리
			otherRb->SetGround(true);

			Transform* otherTr = other->GetOwner()->GetComponent<Transform>();
			Collider* otherCol = other;
			Transform* floorTr = this->GetOwner()->GetComponent<Transform>();
			Collider* floorCol = this->GetOwner()->GetComponent<Collider>();

			Vector2 otherPos = otherTr->GetPosition() + otherCol->GetOffset();
			Vector2 floorPos = floorTr->GetPosition() + floorCol->GetOffset();

			float otherBottom = otherPos.y + (otherCol->GetSize().y * 100.0f / 2.0f);
			float floorTop = floorPos.y - (floorCol->GetSize().y * 100.0f / 2.0f);

			if (otherBottom > floorTop && otherPos.y < floorPos.y)
			{
				Vector2 otherTrPos = otherTr->GetPosition();
				otherTrPos.y -= (otherBottom - floorTop) - 0.01f;
				otherTr->SetPosition(otherTrPos);
			}
		}
	}

	void FloorScript::OnCollisionExit(Collider* other)
	{
		Rigidbody* otherRb = other->GetOwner()->GetComponent<Rigidbody>();
		if (otherRb != nullptr)
		{
			otherRb->SetGround(false);
		}
	}
}