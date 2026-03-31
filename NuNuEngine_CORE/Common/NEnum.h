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
		CurseComponent,
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

	constexpr unsigned int eLayerTypeMax = static_cast<unsigned int>(eLayerType::Max);

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

	enum class eCurseType
	{
		HeavyBody,      // 이동속도 -50%
		Miniaturize,    // 크기 0.3배
		Gigantify,      // 크기 3배
		GlassBody,      // 받는 피해 3배
		FireBody,       // 지속 피해
		Invisible,      // 아군 시야 사라짐
		Magnet,         // 금속 달라붙음
		ForceJump,      // 5초마다 강제 점프
		VisionInvert,   // 상하 시야 반전
		ControlInvert,  // 좌우 이동 반전
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
