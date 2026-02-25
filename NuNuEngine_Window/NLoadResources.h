#pragma once
#include "../NuNuEngine_SOURCE/NResources.h"
#include "../NuNuEngine_SOURCE/NTexture.h"

namespace NuNu
{
	void LoadResources()
	{
		Resources::Load<graphics::Texture>(L"BG", L"../Resources/BlackHole.jpg");
		Resources::Load<graphics::Texture>(L"Hell_BG", L"../Resources/Hell.png");
		Resources::Load<graphics::Texture>(L"Space_BG", L"../Resources/Space.jpg");
		Resources::Load<graphics::Texture>(L"Logi", L"../Resources/Logi.png");
		Resources::Load<graphics::Texture>(L"TestIcon", L"../Resources/testIcon.png");
		Resources::Load<graphics::Texture>(L"tree", L"../Resources/tree.png");

	}
}