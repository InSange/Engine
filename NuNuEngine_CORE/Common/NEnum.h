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
		MeshRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
		CharacterController,
		Collider3D,
		Health,
		End,
	};
	enum class eLayerType
	{
		None,
		BackGround,
		Player,
		Floor,
		Particle,
		Max = 16,
	};

	constexpr UINT eLayerTypeMax = static_cast<UINT>(eLayerType::Max);

	enum class eResourceType
	{
		Texture,
		Mesh,
		Mesh3D,
		Material,
		Animation,
		AudioClip,
		Shader,
		Prefab,
		End,
	};

	enum class eColliderType
	{
		Circle2D,
		Rect2D,
		AABB3D,
		End,
	};

	enum class eUIType
	{
		HpBar,
		Button,
		End,
	};
}
