#pragma once
#include "Common/CommonInclude.h"
#include "Component/Collider/NBoxCollider2D.h"
#include "Component/Collider/NCircleCollider2D.h"

namespace NuNu
{
	using namespace enums;

	union CollisionID
	{
		struct
		{
			UINT32 Left;
			UINT32 Right;
		};

		UINT64 Id;
	};

	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render();
		static void Clear();

		static void CollisionLayerCheck(eLayerType left, eLayerType right, bool enable);
		static void LayerCollision(eLayerType left, eLayerType right);
		static void ColliderCollision(Collider* left, Collider* right);
		static bool Intersect(Collider* left, Collider* right);

	private:
		static std::bitset<CAST_UINT(eLayerType::Max)> mCollisionLayerMatrix[CAST_UINT(eLayerType::Max)];
		static std::unordered_map<UINT64, bool> mCollisionMap;
	};
}


