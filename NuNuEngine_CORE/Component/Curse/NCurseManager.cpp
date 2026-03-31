#include "NCurseManager.h"
#include "NCurseComponent.h"
#include <algorithm>
#include <random>

namespace NuNu
{
	std::vector<enums::eCurseType> CurseManager::BuildPool()
	{
		std::vector<enums::eCurseType> pool;
		pool.reserve((int)enums::eCurseType::End);
		for (int i = 0; i < (int)enums::eCurseType::End; i++)
			pool.push_back((enums::eCurseType)i);
		return pool;
	}

	void CurseManager::AssignCurses(
		const std::vector<CurseComponent*>& players,
		int cursesPerPlayer,
		enums::eCurseType requiredCurse)
	{
		if (players.empty() || cursesPerPlayer <= 0) return;

		std::vector<enums::eCurseType> pool = BuildPool();

		static std::mt19937 rng(std::random_device{}());
		std::shuffle(pool.begin(), pool.end(), rng);

		// 플레이어별 배정 목록
		std::vector<std::vector<enums::eCurseType>> assignments(players.size());

		// 필수 저주 처리 — 랜덤 플레이어에게 먼저 확정 배정
		if (requiredCurse != enums::eCurseType::End)
		{
			auto it = std::find(pool.begin(), pool.end(), requiredCurse);
			if (it != pool.end()) pool.erase(it);

			int target = std::uniform_int_distribution<int>(0, (int)players.size() - 1)(rng);
			assignments[target].push_back(requiredCurse);
		}

		// 나머지 저주를 풀에서 순서대로 배분
		int poolIdx = 0;
		for (int i = 0; i < (int)players.size(); i++)
		{
			int needed = cursesPerPlayer - (int)assignments[i].size();
			for (int j = 0; j < needed && poolIdx < (int)pool.size(); j++)
				assignments[i].push_back(pool[poolIdx++]);
		}

		// CurseComponent에 실제 적용
		for (int i = 0; i < (int)players.size(); i++)
		{
			for (enums::eCurseType curse : assignments[i])
				players[i]->AddCurse(curse);
		}
	}

	void CurseManager::ClearAll(const std::vector<CurseComponent*>& players)
	{
		for (CurseComponent* cc : players)
		{
			if (cc == nullptr) continue;
			const auto curseList = cc->GetCurseList(); // 복사
			for (enums::eCurseType curse : curseList)
				cc->RemoveCurse(curse);
		}
	}
}
