#include "NCurseComponent.h"
#include "Helpers/NTime.h"
#include <algorithm>

namespace NuNu
{
	CurseComponent::CurseComponent()
		: Component(enums::eComponentType::CurseComponent)
		, mForceJumpTimer(0.0f)
	{
	}

	void CurseComponent::Update()
	{
		if (HasCurse(enums::eCurseType::ForceJump))
			mForceJumpTimer += Time::DeltaTime();
	}

	void CurseComponent::AddCurse(enums::eCurseType curse)
	{
		mCurseList.push_back(curse);
	}

	void CurseComponent::RemoveCurse(enums::eCurseType curse)
	{
		auto it = std::find(mCurseList.begin(), mCurseList.end(), curse);
		if (it != mCurseList.end())
			mCurseList.erase(it);
	}

	void CurseComponent::TransferCurse(enums::eCurseType curse, CurseComponent* target)
	{
		if (target == nullptr) return;
		if (!HasCurse(curse))  return;

		RemoveCurse(curse);
		target->AddCurse(curse);
	}

	bool CurseComponent::HasCurse(enums::eCurseType curse) const
	{
		return std::find(mCurseList.begin(), mCurseList.end(), curse) != mCurseList.end();
	}

	float CurseComponent::GetMoveSpeedMultiplier() const
	{
		float mult = 1.0f;
		if (HasCurse(enums::eCurseType::HeavyBody))   mult *= 0.5f;
		if (HasCurse(enums::eCurseType::Gigantify))   mult *= 0.7f; // 거인화 → 다소 느려짐
		if (HasCurse(enums::eCurseType::Miniaturize)) mult *= 1.3f; // 소형화 → 다소 빨라짐
		return mult;
	}

	float CurseComponent::GetIncomingDamageMultiplier() const
	{
		float mult = 1.0f;
		if (HasCurse(enums::eCurseType::GlassBody)) mult *= 3.0f;
		return mult;
	}

	bool CurseComponent::IsControlInverted() const
	{
		return HasCurse(enums::eCurseType::ControlInvert);
	}

	bool CurseComponent::IsVisionInverted() const
	{
		return HasCurse(enums::eCurseType::VisionInvert);
	}

	bool CurseComponent::ConsumeForceJump()
	{
		constexpr float kForceJumpInterval = 5.0f;
		if (HasCurse(enums::eCurseType::ForceJump) && mForceJumpTimer >= kForceJumpInterval)
		{
			mForceJumpTimer = 0.0f;
			return true;
		}
		return false;
	}
}
