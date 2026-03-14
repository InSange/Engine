#pragma once
#include "CommonInclude.h"
#include "NBoxCollider2D.h"
#include "NCircleCollider2D.h"

namespace NuNu
{
	using namespace enums;

	union CollisionID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};

		UINT64 id;
	};

	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hdc);

		static void CollisionLayerCheck(eLayerType left, eLayerType right, bool enable);
		static void LayerCollision(class Scene* scene, eLayerType left, eLayerType right);
		static void ColliderCollision(Collider* left, Collider* right);
		static bool Intersect(Collider* left, Collider* right);

	private:
		static std::bitset <eLayerTypeMax> mCollisionLayerMatrix[eLayerTypeMax];
		static std::unordered_map<UINT64, bool> mCollisionMap;
	};
}

