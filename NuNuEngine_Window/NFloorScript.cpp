#include "NFloorScript.h"
#include "NInput.h"
#include "NTransform.h"
#include "NTime.h"
#include "NGameObject.h"
#include "NAnimator.h"
#include "NObject.h"
#include "NRigidbody.h"
#include "NCollider.h"

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

		Transform* otherTr = other->GetOwner()->GetComponent<Transform>();
		Collider* otherCol = other;

		Rigidbody* floorRb = this->GetOwner()->GetComponent<Rigidbody>();
		Transform* floorTr = this->GetOwner()->GetComponent<Transform>();
		Collider* floorCol = this->GetOwner()->GetComponent<Collider>();

		float len = fabs(otherTr->GetPosition().y - floorTr->GetPosition().y);
		float scale = fabs(otherCol->GetSize().y * 100.0f / 2.0f) + fabs(floorCol->GetSize().y * 100.0f / 2.0f);

		if (len < scale)
		{
			Vector2 otherPos = otherTr->GetPosition();

			if (otherTr->GetPosition().y < floorTr->GetPosition().y)
			{
				otherPos.y -= (scale - len) - 0.1f;
				otherTr->SetPosition(otherPos);

				otherRb->SetGround(true);
			}
		}

//		otherRb->SetGround(true);
	}

	void FloorScript::OnCollisionStay(Collider* other)
	{
		Rigidbody* otherRb = other->GetOwner()->GetComponent<Rigidbody>();
		if (otherRb != nullptr)
		{
			otherRb->SetGround(true);
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