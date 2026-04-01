#pragma once
#include "Component/Component/NComponent.h"
#include "Common/NEnum.h"
#include <vector>

namespace NuNu
{
	class CurseComponent : public Component
	{
	public:
		CurseComponent();
		virtual ~CurseComponent() = default;

		const char* GetComponentKey() const override { return "CurseComponent"; }
		void Update() override;

		void AddCurse(enums::eCurseType curse);
		void RemoveCurse(enums::eCurseType curse);
		void TransferCurse(enums::eCurseType curse, CurseComponent* target);
		bool HasCurse(enums::eCurseType curse) const;

		const std::vector<enums::eCurseType>& GetCurseList() const { return mCurseList; }

		// Modifier queries — CharacterController / Health가 매 프레임 읽는다
		float GetMoveSpeedMultiplier()      const;
		float GetIncomingDamageMultiplier() const;
		bool  IsControlInverted()           const;
		bool  IsVisionInverted()            const;
		bool  ConsumeForceJump();           // ForceJump 저주 발동 시 true 반환 후 타이머 리셋

	private:
		std::vector<enums::eCurseType> mCurseList;
		float mForceJumpTimer; // ForceJump 저주용 누적 시간
	};
}
