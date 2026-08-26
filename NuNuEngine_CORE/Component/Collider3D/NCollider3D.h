#pragma once
#include "Component/Component/NComponent.h"
#include "Common/NMath.h"

namespace NuNu
{
	struct AABB3D
	{
		math::Vector3 Min;
		math::Vector3 Max;

		bool Overlaps(const AABB3D& other) const
		{
			return (Min.x < other.Max.x && Max.x > other.Min.x)
				&& (Min.y < other.Max.y && Max.y > other.Min.y)
				&& (Min.z < other.Max.z && Max.z > other.Min.z);
		}
	};

	class Collider3D : public Component
	{
	public:
		Collider3D();
		virtual ~Collider3D() = default;

		const char* GetComponentKey() const override { return "Collider3D"; }
		void Initialize() override;

		AABB3D GetWorldAABB() const;

		math::Vector3 mOffset;
		math::Vector3 mHalfExtents; // half-size on each axis
	};
}
