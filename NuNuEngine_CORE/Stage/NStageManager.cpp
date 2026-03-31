#include "NStageManager.h"
#include "GameObject/NGameObject.h"
#include "Component/Transform/NTransform.h"
#include "Component/Health/NHealth.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NCurseManager.h"

namespace NuNu
{
	std::vector<GameObject*> StageManager::mPlayers    = {};
	StageData                StageManager::mCurrentStage = {};
	StageManager::eState     StageManager::mState      = StageManager::eState::Idle;

	void StageManager::RegisterPlayer(GameObject* player)
	{
		if (player == nullptr) return;
		mPlayers.push_back(player);
	}

	void StageManager::ClearPlayers()
	{
		mPlayers.clear();
	}

	void StageManager::LoadStage(const StageData& data)
	{
		mCurrentStage = data;
		mState        = eState::InStage;

		// 플레이어 리스폰 + 저주 배분
		std::vector<CurseComponent*> cursedPlayers;
		for (GameObject* p : mPlayers)
		{
			if (p == nullptr) continue;

			// 리스폰 위치
			Transform* tr = p->GetComponent<Transform>();
			if (tr) tr->SetPosition(data.safeSpawnPos);

			// Health 리셋
			Health* hp = p->GetComponent<Health>();
			if (hp) hp->Initialize();

			// 저주 초기화 후 수집
			CurseComponent* cc = p->GetComponent<CurseComponent>();
			if (cc)
			{
				const auto list = cc->GetCurseList();
				for (auto curse : list) cc->RemoveCurse(curse);
				cursedPlayers.push_back(cc);
			}
		}

		// CurseManager로 저주 배분
		if (!cursedPlayers.empty())
		{
			CurseManager::AssignCurses(
				cursedPlayers,
				data.cursesPerPlayer,
				data.requiredCurse);
		}
	}

	void StageManager::Update()
	{
		if (mState != eState::InStage) return;

		for (int i = 0; i < (int)mPlayers.size(); i++)
		{
			if (mPlayers[i] == nullptr) continue;

			Health* hp = mPlayers[i]->GetComponent<Health>();
			if (hp && hp->IsDead())
				respawnPlayer(i);
		}
	}

	void StageManager::OnStageClear()
	{
		if (mState != eState::InStage) return;
		mState = eState::StageClear;
	}

	void StageManager::respawnPlayer(int playerIdx)
	{
		GameObject* p = mPlayers[playerIdx];
		if (p == nullptr) return;

		Transform* tr = p->GetComponent<Transform>();
		if (tr) tr->SetPosition(mCurrentStage.safeSpawnPos);

		Health* hp = p->GetComponent<Health>();
		if (hp) hp->Initialize();
	}
}
