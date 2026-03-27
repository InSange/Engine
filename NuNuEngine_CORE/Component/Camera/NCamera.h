#pragma once
#include "Component/Component/NComponent.h"

#include "Event/NEvent.h"
#include "GameObject/NGameObject.h"

namespace NuNu
{
	using namespace NuNu::math;
	class Camera : public Component
	{
	public:
		enum class eProjectionType
		{
			Perspective,
			Orthographic
		};

		Camera();
		virtual ~Camera();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void CreateViewMatrix();
		void CreateProjectionMatrix(eProjectionType type);

		const Matrix& GetViewMatrix() { return mViewMatrix; }
		const Matrix& GetProjectionMatrix() { return mProjectionMatrix; }

		void SetProjectionType(const eProjectionType type) { mProjectionType = type; }
		void SetSize(const float size) { mSize = size; }

	private:
		eProjectionType mProjectionType;

		Matrix mViewMatrix;
		Matrix mProjectionMatrix;
		float mAspectRatio;
		float mNear;
		float mFar;
		float mSize; 
	};
}


