#include "NEditorCamera.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"

namespace NuNu
{
	EditorCamera::EditorCamera()
		: Camera()
		, mRenderTarget(nullptr)
		, mYaw(0.0f)
		, mPitch(0.0f)
		, mLastMousePos(math::Vector2::Zero)
		, mMoveSpeed(10.0f)
		, mRotateSpeed(0.2f)
	{

	}
	EditorCamera::~EditorCamera()
	{
		SAFE_DELETE(mRenderTarget);
	}

	void EditorCamera::Initialize()
	{
		Camera::Initialize();
	}

	void EditorCamera::Update()
	{
		Camera::Update();

		math::Vector2 curPos = Input::GetMousePosition();

		if (Input::GetKey(eKeyCode::RButton))
		{
			math::Vector2 delta = curPos - mLastMousePos;

			mYaw   += delta.x * mRotateSpeed;
			mPitch += delta.y * mRotateSpeed;
			if (mPitch >  89.0f) mPitch =  89.0f;
			if (mPitch < -89.0f) mPitch = -89.0f;

			Transform* tr = GetOwner()->GetComponent<Transform>();
			tr->SetRotation(mPitch, mYaw, 0.0f);

			float speed = mMoveSpeed * Time::DeltaTime();
			math::Vector3 pos = tr->GetPosition();

			if (Input::GetKey(eKeyCode::W)) pos += tr->Forward() * speed;
			if (Input::GetKey(eKeyCode::S)) pos -= tr->Forward() * speed;
			if (Input::GetKey(eKeyCode::A)) pos -= tr->Right()   * speed;
			if (Input::GetKey(eKeyCode::D)) pos += tr->Right()   * speed;
			if (Input::GetKey(eKeyCode::E)) pos += tr->Up()      * speed;
			if (Input::GetKey(eKeyCode::Q)) pos -= tr->Up()      * speed;

			tr->SetPosition(pos);
		}

		mLastMousePos = curPos;
	}

	void EditorCamera::LateUpdate()
	{
		Camera::LateUpdate();
	}

	void EditorCamera::Render(const Matrix& view, const Matrix& projection)
	{
		Camera::Render(view, projection);
	}

	void EditorCamera::OnEvent(Event& e)
	{

	}

	void EditorCamera::CreateRenderTarget(UINT width, UINT height)
	{
		if (mRenderTarget != nullptr)
			delete mRenderTarget;

		graphics::RenderTargetSpecification spec;
		spec.Width = width;
		spec.Height = height;

		spec.Attachments = { graphics::eRenderTragetFormat::RGBA8, graphics::eRenderTragetFormat::Depth };

		mRenderTarget = new graphics::RenderTarget(spec);
	}
}