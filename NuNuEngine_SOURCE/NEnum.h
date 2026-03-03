#pragma once

namespace NuNu::enums
{
	enum class eComponentType
	{
		Trnasform,
		SpriteRenderer,
		Animator,
		Script,
		Camera,
		End,
	};
	enum class eLayerType
	{
		None,
		BackGround,
		UI,
		Player,
		Particle,
		Max = 16,
	};

	enum class eResourceType
	{
		Texture,
		AudioClip,
		Animation,
		Prefab,
		End,
	};
}