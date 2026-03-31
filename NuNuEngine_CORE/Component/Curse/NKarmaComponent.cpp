#include "NKarmaComponent.h"
#include "NCurseComponent.h"
#include "NCurseManager.h"
#include "Component/Health/NHealth.h"
#include "GameObject/NGameObject.h"
#include "Helpers/NTime.h"
#include <algorithm>
#include <cstdlib>

namespace NuNu
{
	KarmaComponent::KarmaComponent()
		: Component(enums::eComponentType::KarmaComponent)
		, mKarma(0.0f)
		, mMaxKarma(kMaxKarma)
		, mSpeedBoostTimer(0.0f)
		, mCurseImmuneTimer(0.0f)
		, mStunTimer(0.0f)
		, mDoTDamageThisFrame(0.0f)
		, mBleedTimer(0.0f)
		, mPoisonTimer(0.0f)
		, mLastAbility(enums::eAbilityType::End)
	{
	}

	void KarmaComponent::Update()
	{
		float dt = Time::DeltaTime();

		// 저주 수에 비례한 카르마 자동 적립
		CurseComponent* cc = GetOwner()->GetComponent<CurseComponent>();
		if (cc)
		{
			float curseCount = static_cast<float>(cc->GetCurseList().size());
			AddKarma(curseCount * kKarmaPerCursePerSec * dt);
		}

		// 버프/디버프 타이머 감소
		if (mSpeedBoostTimer  > 0.0f) mSpeedBoostTimer  -= dt;
		if (mCurseImmuneTimer > 0.0f) mCurseImmuneTimer -= dt;
		if (mStunTimer        > 0.0f) mStunTimer        -= dt;

		// DoT 처리
		mDoTDamageThisFrame = 0.0f;

		if (mBleedTimer > 0.0f)
		{
			mBleedTimer -= dt;
			mDoTDamageThisFrame += 5.0f * dt;
		}
		if (mPoisonTimer > 0.0f)
		{
			mPoisonTimer -= dt;
			mDoTDamageThisFrame += 3.0f * dt;
		}

		// DoT를 Health에 직접 적용
		if (mDoTDamageThisFrame > 0.0f)
		{
			Health* hp = GetOwner()->GetComponent<Health>();
			if (hp) hp->TakeDamage(mDoTDamageThisFrame);
		}
	}

	void KarmaComponent::AddKarma(float amount)
	{
		mKarma = std::min(mKarma + amount, mMaxKarma);
	}

	enums::eAbilityType KarmaComponent::SpendKarma()
	{
		if (mKarma < kKarmaCostPerSpend)
			return enums::eAbilityType::End;

		mKarma -= kKarmaCostPerSpend;
		enums::eAbilityType ability = pickRandomAbility();
		applyAbility(ability);
		mLastAbility = ability;
		return ability;
	}

	enums::eAbilityType KarmaComponent::pickRandomAbility() const
	{
		// 좋음 5 + 나쁨 5 + 상황적 3 = 13가지
		int idx = std::rand() % static_cast<int>(enums::eAbilityType::End);
		return static_cast<enums::eAbilityType>(idx);
	}

	void KarmaComponent::applyAbility(enums::eAbilityType ability)
	{
		using enums::eAbilityType;
		using enums::eCurseType;

		Health*          hp = GetOwner()->GetComponent<Health>();
		CurseComponent*  cc = GetOwner()->GetComponent<CurseComponent>();

		switch (ability)
		{
		case eAbilityType::HealSmall:
			if (hp) hp->Heal(20.0f);
			break;
		case eAbilityType::HealFull:
			if (hp) hp->Heal(hp->GetMaxHp());
			break;
		case eAbilityType::CurseRemove:
			if (cc && !cc->GetCurseList().empty())
				cc->RemoveCurse(cc->GetCurseList().front());
			break;
		case eAbilityType::SpeedBoostTemp:
			mSpeedBoostTimer = 5.0f;
			break;
		case eAbilityType::CurseImmuneTemp:
			mCurseImmuneTimer = 5.0f;
			break;
		case eAbilityType::AddRandomCurse:
			if (cc)
			{
				int curseIdx = std::rand() % static_cast<int>(eCurseType::End);
				cc->AddCurse(static_cast<eCurseType>(curseIdx));
			}
			break;
		case eAbilityType::BleedDoT:
			mBleedTimer = 5.0f;
			break;
		case eAbilityType::PoisonDoT:
			mPoisonTimer = 10.0f;
			break;
		case eAbilityType::SleepStun:
			mStunTimer = 2.0f;
			break;
		case eAbilityType::AreaExplosion:
			if (hp) hp->TakeDamage(30.0f);
			break;
		case eAbilityType::TransferCurseToFriend:
			// 파트너 연동은 Phase 4 (네트워크) 이후 구현
			// 현재는 로컬 테스트용 — 내 저주 1개 제거로 대체
			if (cc && !cc->GetCurseList().empty())
				cc->RemoveCurse(cc->GetCurseList().front());
			break;
		case eAbilityType::TeleportRandom:
			// 텔레포트는 StageManager에서 안전지대 위치를 받아야 함 — 추후 연동
			break;
		case eAbilityType::WeaponItem:
			// 무기 아이템 시스템 미구현 — 추후 연동
			break;
		default:
			break;
		}
	}
}
