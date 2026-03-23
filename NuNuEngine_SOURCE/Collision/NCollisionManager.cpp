#include "Collision/NCollisionManager.h"
#include "Scene/NScene.h"
#include "Scene/NSceneManager.h"
#include "GameObject/NGameObject.h"
#include "Component/Collider/NCollider.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"

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

