#pragma once
#include "Common/NEnum.h"
#include <vector>

namespace NuNu
{
	class CurseComponent;

	class CurseManager
	{
	public:
		// 스테이지 시작 시 플레이어들에게 저주 배분
		// requiredCurse: 두 플레이어 중 반드시 한 명에게 배정할 저주 (End = 없음)
		static void AssignCurses(
			const std::vector<CurseComponent*>& players,
			int cursesPerPlayer,
			enums::eCurseType requiredCurse = enums::eCurseType::End);

		// 모든 플레이어의 저주 초기화
		static void ClearAll(const std::vector<CurseComponent*>& players);

	private:
		static std::vector<enums::eCurseType> BuildPool();
	};
}
