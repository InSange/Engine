#pragma once
#include "Common/NEnum.h"
#include "Common/NMath.h"
#include <vector>

namespace NuNu
{
	class GameObject;

	struct StageData
	{
		int               stageId         = 0;
		int               cursesPerPlayer  = 1;
		enums::eCurseType requiredCurse    = enums::eCurseType::End; // End = 없음
		math::Vector3     safeSpawnPos     = math::Vector3(0.0f, 1.7f, -6.0f); // 출발 Safe Area 리스폰 위치
	};

	class StageManager
	{
	public:
		enum class eState { Idle, InStage, StageClear };

		// 플레이어 오브젝트 등록 (Health + CurseComponent + Transform 보유 필요)
		static void RegisterPlayer(GameObject* player);
		static void ClearPlayers();

		// 스테이지 로드 — CurseManager로 저주 배분 후 InStage 진입
		static void LoadStage(const StageData& data);

		// 매 프레임 — Health 사망 감지 → 자동 리스폰
		static void Update();

		// 외부(트리거 등)에서 클리어 조건 달성 시 호출
		static void OnStageClear();

		static eState           GetState()        { return mState; }
		static const StageData& GetCurrentStage() { return mCurrentStage; }

	private:
		static void respawnPlayer(int playerIdx);

		static std::vector<GameObject*> mPlayers;
		static StageData                mCurrentStage;
		static eState                   mState;
	};
}
