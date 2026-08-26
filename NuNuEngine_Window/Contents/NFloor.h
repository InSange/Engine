#pragma once
#include <GameObject/NGameObject.h>

namespace NuNu
{
	class Floor : public GameObject
	{
	public:
		Floor();
		~Floor();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

	private:

	};
}


