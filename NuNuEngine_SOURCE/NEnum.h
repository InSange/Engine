#pragma once

namespace NuNu::enums
{
	enum class eLayerType
	{
		None,
		BackGround,
		UI,
		Player,
		Max = 16,
	};

	enum class eResourceType
	{
		Texture,
		AudioClip,
		Prefab,
		End,
	};
}