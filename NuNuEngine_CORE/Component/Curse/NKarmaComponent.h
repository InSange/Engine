#pragma once
#include "Component/Component/NComponent.h"
#include "Common/NEnum.h"
#include <functional>
#include <vector>

namespace NuNu
{
	class CurseComponent;
	class Health;

	class KarmaComponent : public Component
	{
	public:
		KarmaComponent();
		virtual ~KarmaComponent() = default;

		const char* GetComponentKey() const override { return "KarmaComponent"; }
		void Update() override;

		// 카르마 수동 적립 (저주 위임 시 즉시 보상 등)
		void AddKarma(float amount);

		// 카르마를 소비하여 랜덤 능력 발동 — 반환값: 발동된 능력 (End = 카르마 부족)
		enums::eAbilityType SpendKarma();

		float GetKarma()    const { return mKarma; }
		float GetMaxKarma() const { return mMaxKarma; }

		// SpeedBoost 적용 여부 — CharacterController가 매 프레임 읽는다
		bool  IsSpeedBoosted()     const { return mSpeedBoostTimer > 0.0f; }
		// CurseImmune 적용 여부 — CurseComponent::TransferCurse에서 수신 차단용
		bool  IsCurseImmune()      const { return mCurseImmuneTimer > 0.0f; }
		// 스턴(행동 불능) — CharacterController가 매 프레임 읽는다
		bool  IsStunned()          const { return mStunTimer > 0.0f; }
		// DoT 데미지 — Update()에서 Health에 직접 적용
		float GetDoTDamageThisFrame() const { return mDoTDamageThisFrame; }

		// 마지막으로 발동된 능력 (에디터 표시용)
		enums::eAbilityType GetLastAbility() const { return mLastAbility; }

		static constexpr float kKarmaCostPerSpend = 25.0f;
		static constexpr float kKarmaPerCursePerSec = 1.0f;
		static constexpr float kMaxKarma = 100.0f;

	private:
		enums::eAbilityType pickRandomAbility() const;
		void applyAbility(enums::eAbilityType ability);

		float mKarma;
		float mMaxKarma;

		// 시간 제한 버프/디버프
		float mSpeedBoostTimer;
		float mCurseImmuneTimer;
		float mStunTimer;

		// DoT — 이번 프레임에 Health에 가할 누적 데미지
		float mDoTDamageThisFrame;
		float mBleedTimer;
		float mPoisonTimer;

		enums::eAbilityType mLastAbility;
	};
}
