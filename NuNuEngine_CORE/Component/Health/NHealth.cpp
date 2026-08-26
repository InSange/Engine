#include "NHealth.h"

namespace NuNu
{
	Health::Health()
		: Component(enums::eComponentType::Health)
		, mMaxHp(100.0f)
		, mHp(100.0f)
		, mbDead(false)
	{
	}

	void Health::Initialize()
	{
		Component::Initialize();
		mHp    = mMaxHp;
		mbDead = false;
	}

	void Health::TakeDamage(float amount)
	{
		if (mbDead) return;

		mHp -= amount;
		if (mHp <= 0.0f)
		{
			mHp    = 0.0f;
			mbDead = true;
		}
	}

	void Health::Heal(float amount)
	{
		if (mbDead) return;

		mHp += amount;
		if (mHp > mMaxHp)
			mHp = mMaxHp;
	}
}
