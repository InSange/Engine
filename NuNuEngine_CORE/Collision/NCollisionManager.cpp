#include "Collision/NCollisionManager.h"
#include "Scene/NScene.h"
#include "Scene/NSceneManager.h"
#include "GameObject/NGameObject.h"
#include "Component/Collider/NCollider.h"
#include "Component/Transform/NTransform.h"
#include "Component/Collider3D/NCollider3D.h"
#include "Component/CharacterController/NCharacterController.h"

namespace NuNu
{
	std::bitset <eLayerTypeMax> CollisionManager::mCollisionLayerMatrix[eLayerTypeMax] = {};
	std::unordered_map<UINT64, bool> CollisionManager::mCollisionMap = {};

	void CollisionManager::Initialize()
	{

	}

	void CollisionManager::Update()
	{
		for (UINT row = 0; row < (UINT)eLayerType::Max; row++)
		{
			for (UINT col = 0; col < (UINT)eLayerType::Max; col++)
			{
				if (mCollisionLayerMatrix[row][col] == true)
				{
					LayerCollision((eLayerType)row, (eLayerType)col);
				}
			}
		}
	}

	void CollisionManager::LateUpdate()
	{
	}

	void CollisionManager::Render()
	{
	}

	void CollisionManager::Clear()
	{
		mCollisionMap.clear();
		mCollisionLayerMatrix->reset();
	}

	void CollisionManager::CollisionLayerCheck(eLayerType left, eLayerType right, bool enable)
	{
		int row = 0;
		int col = 0;

		if (left <= right)
		{
			row = (UINT)left;
			col = (UINT)right;
		}
		else
		{
			row = (UINT)right;
			col = (UINT)left;
		}

		mCollisionLayerMatrix[row][col] = enable;
	}

	void CollisionManager::LayerCollision(eLayerType left, eLayerType right)
	{
		const std::vector<GameObject*>& leftObjs = SceneManager::GetGameObjects(left);
		const std::vector<GameObject*>& rightObjs = SceneManager::GetGameObjects(right);


		for (GameObject* leftObj : leftObjs)
		{
			if (leftObj->IsActive() == false)
				continue;

			Collider* leftCol = leftObj->GetComponent<Collider>();
			if (leftCol == nullptr)
				continue;

			for (GameObject* rightObj : rightObjs)
			{
				if (rightObj->IsActive() == false)
					continue;

				Collider* rightCol = rightObj->GetComponent<Collider>();
				if (rightCol == nullptr)
					continue;
				if (leftObj == rightObj)
					continue;

				ColliderCollision(leftCol, rightCol);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		CollisionID id = {};
		id.Left = left->GetID();
		id.Right = right->GetID();

		auto iter = mCollisionMap.find(id.Id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(id.Id, false));
			iter = mCollisionMap.find(id.Id);
		}

		if (Intersect(left, right))
		{	// 충돌 체크
			if (iter->second == false)	// 충돌하지 않음
			{	// 충돌 시작
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				iter->second = true;
			}
			else
			{	// 충돌 중
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			if (iter->second == true)	// 충돌하고 있으면?
			{	// 충돌 끝
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
				iter->second = false;
			}
		}
	}

	void CollisionManager::Resolve3D(CharacterController* cc)
	{
		if (cc == nullptr) return;

		Transform* tr = cc->GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();

		const float r = cc->mCapsuleRadius;
		const float h = cc->mCapsuleHeight;

		// 터미널 속도 제한 — 터널링 방지
		constexpr float kMaxFallSpeed = 30.0f;
		if (cc->mVelocityY < -kMaxFallSpeed) cc->mVelocityY = -kMaxFallSpeed;

		cc->mbGrounded = false;

		math::Vector3 pMin(pos.x - r, pos.y,     pos.z - r);
		math::Vector3 pMax(pos.x + r, pos.y + h, pos.z + r);

		for (UINT layer = 0; layer < (UINT)eLayerType::Max; layer++)
		{
			std::vector<GameObject*> objs = SceneManager::GetGameObjects((eLayerType)layer);
			for (GameObject* obj : objs)
			{
				if (obj == cc->GetOwner() || !obj->IsActive()) continue;

				Collider3D* col = obj->GetComponent<Collider3D>();
				if (col == nullptr) continue;

				AABB3D cAABB = col->GetWorldAABB();

				if (pMax.x <= cAABB.Min.x || pMin.x >= cAABB.Max.x) continue;
				if (pMax.y <= cAABB.Min.y || pMin.y >= cAABB.Max.y) continue;
				if (pMax.z <= cAABB.Min.z || pMin.z >= cAABB.Max.z) continue;

				float ox = std::min(pMax.x, cAABB.Max.x) - std::max(pMin.x, cAABB.Min.x);
				float oy = std::min(pMax.y, cAABB.Max.y) - std::max(pMin.y, cAABB.Min.y);
				float oz = std::min(pMax.z, cAABB.Max.z) - std::max(pMin.z, cAABB.Min.z);

				if (oy <= ox && oy <= oz)
				{
					// velocity 방향으로 floor/ceiling 판단 (center 비교는 터널링 시 오판)
					if (cc->mVelocityY <= 0.0f)
					{
						// 낙하 또는 정지 → 바닥 위로 스냅
						pos.y = cAABB.Max.y;
						cc->mVelocityY = 0.0f;
						cc->mbGrounded = true;
					}
					else
					{
						// 상승 → 천장에서 튕김
						pos.y = cAABB.Min.y - h;
						cc->mVelocityY = 0.0f;
					}
				}
				else if (ox <= oz)
				{
					float pCenterX = (pMin.x + pMax.x) * 0.5f;
					float cCenterX = (cAABB.Min.x + cAABB.Max.x) * 0.5f;
					if (pCenterX >= cCenterX) pos.x += ox;
					else                      pos.x -= ox;
				}
				else
				{
					float pCenterZ = (pMin.z + pMax.z) * 0.5f;
					float cCenterZ = (cAABB.Min.z + cAABB.Max.z) * 0.5f;
					if (pCenterZ >= cCenterZ) pos.z += oz;
					else                      pos.z -= oz;
				}

				// 해소 후 AABB 갱신
				pMin = math::Vector3(pos.x - r, pos.y,     pos.z - r);
				pMax = math::Vector3(pos.x + r, pos.y + h, pos.z + r);
			}
		}

		tr->SetPosition(pos);
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Vector2 leftPos = leftTr->GetPosition() + left->GetOffset();
		Vector2 rightPos = rightTr->GetPosition() + right->GetOffset();
		leftTr->GetScale();
		Vector2 leftSize = left->GetSize() * 100.0f;
		Vector2 rightSize = right->GetSize() * 100.0f;

		enums::eColliderType leftType = left->GetColliderType();
		enums::eColliderType rightType = right->GetColliderType();

		if (leftType == enums::eColliderType::Rect2D && rightType == enums::eColliderType::Rect2D)
		{
			// rect - rect
			if (fabs(leftPos.x - rightPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f) && fabs(leftPos.y - rightPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f))
			{
				return true;
			}
		}

		if (leftType == enums::eColliderType::Circle2D && rightType == enums::eColliderType::Circle2D)
		{
			// circle - circle

			Vector2 leftCirclePos = leftPos + (leftSize / 2.0f);
			Vector2 rightCirclePos = rightPos + (rightSize / 2.0f);

			float dist = (leftCirclePos - rightCirclePos).Length();
			if (dist <= (leftSize.x / 2.0f + rightSize.x / 2.0f))
			{
				return true;
			}
		}

		if ((leftType == enums::eColliderType::Circle2D && rightType == enums::eColliderType::Rect2D)
			|| (leftType == enums::eColliderType::Rect2D && rightType == enums::eColliderType::Circle2D))
		{
			Collider* circleCol = (leftType == enums::eColliderType::Circle2D) ? left : right;
			Collider* rectCol = (leftType == enums::eColliderType::Rect2D) ? left : right;

			Transform* circleTr = circleCol->GetOwner()->GetComponent<Transform>();
			Transform* rectTr = rectCol->GetOwner()->GetComponent<Transform>();

			Vector2 circlePos = circleTr->GetPosition() + circleCol->GetOffset();
			Vector2 rectPos = rectTr->GetPosition() + rectCol->GetOffset();

			Vector2 circleSize = circleCol->GetSize() * 100.0f;// circleTr->GetScale();
			Vector2 rectSize = rectCol->GetSize() * 100.0f;// rectTr->GetScale();

			float radius = circleSize.x / 2.0f;

			float rectLeft = rectPos.x - (rectSize.x / 2.0f);
			float rectRight = rectPos.x + (rectSize.x / 2.0f);
			float rectTop = rectPos.y - (rectSize.y / 2.0f);
			float rectBottom = rectPos.y + (rectSize.y / 2.0f);

			float closestX = std::max(rectLeft, std::min(circlePos.x, rectRight));
			float closestY = std::max(rectTop, std::min(circlePos.y, rectBottom));

			Vector2 closestPoint = Vector2(closestX, closestY);
			float distance = (circlePos - closestPoint).Length();

			if (distance <= radius)
			{
				return true;
			}
		}

		return false;
	}
}

