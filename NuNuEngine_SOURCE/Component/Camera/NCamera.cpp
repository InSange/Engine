#include "Component/Camera/NCamera.h"
#include "GameObject/NGameObject.h"
#include "Component/Transform/NTransform.h"
#include "High Level Interface/NApplication.h"

extern NuNu::Application application;

namespace NuNu
{
	Camera::Camera()
		: Component(enums::eComponentType::Camera)
		, mDistance(Vector2::Zero)
		, mResolution(Vector2::Zero)
		, mLookPosition(Vector2::Zero)
		, mTarget(nullptr)
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mResolution.x = application.GetWidth();
		mResolution.y = application.GetHeight();
	}
	void Camera::Update()
	{
		if (mTarget)
		{
			Transform* tr = mTarget->GetComponent<Transform>();
			mLookPosition = tr->GetPosition();
		}
		else
		{
			Transform* cameraTr = GetOwner()->GetComponent<Transform>();
			mLookPosition = cameraTr->GetPosition();
		}

		mDistance = mLookPosition - (mResolution / 2.0f);
	}
	void Camera::LateUpdate()
	{
		// 오브젝트들 위치 보정
	}
	void Camera::Render(HDC hdc)
	{
	}
}
