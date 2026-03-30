#pragma once
#include "Component/Component/NComponent.h"

namespace NuNu
{
	class Health : public Component
	{
	public:
		Health();
		virtual ~Health() = default;

		void Initialize() override;

		void TakeDamage(float amount);
		void Heal(float amount);

		float GetHp()    const { return mHp; }
		float GetMaxHp() const { return mMaxHp; }
		bool  IsDead()   const { return mbDead; }

		float mMaxHp;

	private:
		float mHp;
		bool  mbDead;
	};
}
