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
		Transform* otherTr = other->GetOwner()->GetComponent<Transform>();
		Collider* otherCol = other;

		Rigidbody* floorRb = this->GetOwner()->GetComponent<Rigidbody>();
		Transform* floorTr = this->GetOwner()->GetComponent<Transform>();
		Collider* floorCol = this->GetOwner()->GetComponent<Collider>();

		float len = fabs(otherTr->GetPosition().y - floorTr->GetPosition().y);
		float scale = fabs(otherCol->GetSize().y * 100 / 2.0f - floorCol->GetSize().y * 100 / 2.0f);

		if (len < scale)
		{
			Vector2 otherPos = otherTr->GetPosition();
			otherPos.y -= (scale - len) - 1.0f;

			otherTr->SetPosition(otherPos);
		}

		otherRb->SetGround(true);
	}

	void FloorScript::OnCollisionStay(Collider* other)
	{
	}

	void FloorScript::OnCollisionExit(Collider* other)
	{
	}
}