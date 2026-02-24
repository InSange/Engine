#include "NSpriteRenderer.h"
#include "NTransform.h"
#include "NGameObject.h"

namespace NuNu
{
	SpriteRenderer::SpriteRenderer()
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::Initialize()
	{
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::LateUpdate()
	{
	}

	void SpriteRenderer::Render(HDC hdc)
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); // 반환값 HGDIOBJ 이전 브러쉬 밀어냄

		/*		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
				HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
				// 기본으로 자주 사용 되는 GDI오브젝트를 미리 DC안에 만들어둔 오브젝트 = 스톡 오브젝트.
				HBRUSH grayBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
				oldBrush = (HBRUSH)SelectObject(hdc, grayBrush);
				*/

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Rectangle(hdc, tr->GetX(), tr->GetY(), 100 + tr->GetX(), 100 + tr->GetY());

		SelectObject(hdc, oldBrush);
		//SelectObject(hdc, oldPen);
		DeleteObject(brush); // 메모리 누수 방지
		//DeleteObject(redPen); // 메모리 누수 방지
	}
}
