#pragma once
#include "Component/Component/NComponent.h"

namespace NuNu
{
	class Collider;
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();
		
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

	private:

	};
}


