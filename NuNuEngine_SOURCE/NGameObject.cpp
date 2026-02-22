#include "NGameObject.h"
#include "NInput.h"

namespace NuNu
{

	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Update()
	{
		if (Input::GetKey(eKeyCode::A))
		{
			mX -= 0.01f;
		}

		if (Input::GetKey(eKeyCode::D))
		{
			mX += 0.01f;
		}

		if (Input::GetKey(eKeyCode::W))
		{
			mY -= 0.01f;
		}

		if (Input::GetKey(eKeyCode::S))
		{
			mY += 0.01f;
		}
	}

	void GameObject::LateUpdate()
	{
	}

	void GameObject::Render(HDC hdc)
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); // 반환값 HGDIOBJ 이전 브러쉬 밀어냄

		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
		// 기본으로 자주 사용 되는 GDI오브젝트를 미리 DC안에 만들어둔 오브젝트 = 스톡 오브젝트.
		HBRUSH grayBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hdc, grayBrush);

		Rectangle(hdc, 100 + mX, 100 + mY, 200 + mX, 200 + mY);

		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(brush); // 메모리 누수 방지
		DeleteObject(redPen); // 메모리 누수 방지

		Ellipse(hdc, 50 + cX, 50 + cY, 75 + cX, +75 + cY);
	}
}