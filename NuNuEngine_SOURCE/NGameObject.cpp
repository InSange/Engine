#include "NGameObject.h"
#include "NInput.h"
#include "NTime.h"
#include "NApplication.h"

namespace NuNu
{
#pragma region GameObject - Player
	GameObject::GameObject()
		: mX(0)
		, mY(0)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Update()
	{
		const int speed = 100.0f;
		if (Input::GetKey(eKeyCode::A))
		{
			mX -= speed * Time::DeltaTime();
		}

		if (Input::GetKey(eKeyCode::D))
		{
			mX += speed * Time::DeltaTime();
		}

		if (Input::GetKey(eKeyCode::W))
		{
			mY -= speed * Time::DeltaTime();
		}

		if (Input::GetKey(eKeyCode::S))
		{
			mY += speed * Time::DeltaTime();
		}

		if (Input::GetKeyDown(eKeyCode::SpaceBar))
		{
			BulletObject* bullet = new BulletObject();
			bullet->SetPosition(this->mX, this->mY);
		}
	}

	void GameObject::LateUpdate()
	{
	}

	void GameObject::Render(HDC hdc)
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); // 반환값 HGDIOBJ 이전 브러쉬 밀어냄

		/*		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
				HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
				// 기본으로 자주 사용 되는 GDI오브젝트를 미리 DC안에 만들어둔 오브젝트 = 스톡 오브젝트.
				HBRUSH grayBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
				oldBrush = (HBRUSH)SelectObject(hdc, grayBrush);*/

		Rectangle(hdc, 100 + mX, 100 + mY, 200 + mX, 200 + mY);

		SelectObject(hdc, oldBrush);
		//SelectObject(hdc, oldPen);
		DeleteObject(brush); // 메모리 누수 방지
		//DeleteObject(redPen); // 메모리 누수 방지
	}
#pragma endregion

#pragma region BulletObject
	BulletObject::BulletObject()
	{
	}
	BulletObject::~BulletObject()
	{
	}
	void BulletObject::Update()
	{
		mY -= 10.0f * Time::DeltaTime();;
	}
	void BulletObject::LateUpdate()
	{
	}
	void BulletObject::Render(HDC hdc)
	{
		Ellipse(hdc, 125 + mX, 125 + mY, 175 + mX, 175 + mY);
	}
#pragma endregion
}