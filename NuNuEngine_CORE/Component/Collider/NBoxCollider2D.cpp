#include "Component/Collider/NBoxCollider2D.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"

namespace NuNu
{
	BoxCollider2D::BoxCollider2D()
		: Collider(eColliderType::Rect2D)
	{
	}
	BoxCollider2D::~BoxCollider2D()
	{
	}
	void BoxCollider2D::Initialize()
	{
	}
	void BoxCollider2D::Update()
	{
	}
	void BoxCollider2D::LateUpdate()
	{
	}
	void BoxCollider2D::Render(const Matrix& view, const Matrix& projection)
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
	}
}
