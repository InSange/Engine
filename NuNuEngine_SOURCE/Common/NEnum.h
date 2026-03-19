#pragma once

namespace NuNu::enums
{
	enum class eComponentType
	{
		Transform,
		Collider,
		Rigidbody,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
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
		Floor,
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

	enum class eUIType
	{
		HpBar,
		Button,
		End,
	};
}