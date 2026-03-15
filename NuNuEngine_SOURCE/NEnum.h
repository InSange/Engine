#pragma once

namespace NuNu::enums
{
	enum class eComponentType
	{
		Trnasform,
		Collider,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		End,
	};
	enum class eLayerType
	{
		None,
		BackGround,
		Tile,
		UI,
		Enemy,
		Player,
		Particle,
		Max = 16,
	};

	constexpr UINT eLayerTypeMax = static_cast<UINT>(eLayerType::Max);

	enum class eResourceType
	{
		Texture,
		AudioClip,
		Animation,
		Prefab,
		End,
	};

	enum class eColliderType
	{
		Circle2D,
		Rect2D,
		End,
	};
}