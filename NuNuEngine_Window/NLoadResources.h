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
		Resources::Load<graphics::Texture>(L"PinkCharacter0", L"../Resources/Character/PinkCharacter/00_PinkCharacter.png");

		Resources::Load<graphics::Texture>(L"Player", L"../Resources/Player/player.png");
		Resources::Load<graphics::Texture>(L"Demon", L"../Resources/Monster/Demon.png");
		Resources::Load<graphics::Texture>(L"MapleEffect", L"../Resources/Player/MapleEffect.png");

		Resources::Load<graphics::Texture>(L"Overworld", L"../Resources/BackGround/Overworld.png");
		Resources::Load<graphics::Texture>(L"Dungeon", L"../Resources/BackGround/dungeon_tiles.png");

		Resources::Load<graphics::Texture>(L"HPBAR", L"..\\Resources\\HPBAR.bmp");
	}
}