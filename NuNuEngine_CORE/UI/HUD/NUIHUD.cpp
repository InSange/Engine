#include "UI/HUD/NUIHUD.h"
#include "Resource/NResources.h"

namespace NuNu
{
	UIHUD::UIHUD()
		: UIBase(eUIType::HpBar)
	{
	}

	UIHUD::~UIHUD()
	{
	}

	void UIHUD::OnInit()
	{
		mTexture = Resources::Find<graphics::Texture>(L"HPBAR");
	}

	void UIHUD::OnActive()
	{
	}

	void UIHUD::OnInActive()
	{
	}

	void UIHUD::OnUpdate()
	{
	}

	void UIHUD::OnLateUpdate()
	{
	}

	void UIHUD::OnRender()
	{
/*		TransparentBlt(hdc
			, 0, 0
			, mTexture->GetWidth()
			, mTexture->GetHeight()
			, mTexture->GetHdc()
			, 0, 0
			, mTexture->GetWidth() /// 2.0f
			, mTexture->GetHeight()
			, RGB(255, 0, 255));*/
	}

	void UIHUD::OnClear()
	{
	}
}

