#pragma once
#include "Component/Script/NScript.h"
#include "Resource/Texture/NTexture.h"

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
		~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
		
		void AttackEnd();

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;

		void SetPixelMapTexture(graphics::Texture* texture) { mPixelMap = texture; }

	private:
		void idle();
		void move();
		void attack();
	private:
		float animTime;
		eState mState;
		class Animator* mAnimator;

		graphics::Texture* mPixelMap;

		
		/*void(*StartEvent)();
		void(*CompeleteEvent)();
		void(*EndEvent)();*/
	};
}


