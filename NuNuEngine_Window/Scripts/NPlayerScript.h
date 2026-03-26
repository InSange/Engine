#pragma once
#include "Component/Script/NScript.h"
#include "Resource/Texture/NTexture.h"
#include "../Contents/NProjectTile.h"

namespace NuNu
{
	class PlayerScript : public Script
	{
	public:
		enum class eState
		{
			Idle,
			Walk,
			Attack,
		};

		PlayerScript();
		virtual ~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;
		
		void AttackEnd();

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;
	private:
		void idle();
		void move();
		void attack();
	private:
		float animTime;
		eState State;
		class Animator* mAnimator;

		ProjectTile* mProjTile;
	};
}


