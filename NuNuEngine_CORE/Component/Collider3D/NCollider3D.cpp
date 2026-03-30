#include "NCollider3D.h"
#include "GameObject/NGameObject.h"
#include "Component/Transform/NTransform.h"

namespace NuNu
{
	Collider3D::Collider3D()
		: Component(enums::eComponentType::Collider3D)
		, mOffset(math::Vector3::Zero)
		, mHalfExtents(0.5f, 0.5f, 0.5f)
	{
	}

	void Collider3D::Initialize()
	{
		Component::Initialize();
	}

	AABB3D Collider3D::GetWorldAABB() const
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 center = tr->GetPosition() + mOffset;
		return AABB3D{ center - mHalfExtents, center + mHalfExtents };
	}
}
