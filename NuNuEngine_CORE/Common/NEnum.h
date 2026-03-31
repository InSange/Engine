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
		KarmaComponent,
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

	enum class eAbilityType
	{
		// Good
		HealSmall,         // HP +20
		HealFull,          // HP 완전 회복
		CurseRemove,       // 내 저주 1개 제거
		SpeedBoostTemp,    // 5초간 이동속도 +50%
		CurseImmuneTemp,   // 5초간 저주 위임 수신 방어
		// Bad
		AddRandomCurse,    // 내 저주 1개 추가
		BleedDoT,          // 5초간 초당 5 출혈 데미지
		PoisonDoT,         // 10초간 초당 3 독 데미지
		SleepStun,         // 2초간 행동 불능
		AreaExplosion,     // 주변 폭발 (자신 포함 데미지)
		// Situational
		TransferCurseToFriend, // 내 저주 1개를 파트너에게 강제 위임
		TeleportRandom,    // 맵 내 랜덤 위치로 텔레포트
		WeaponItem,        // 랜덤 무기 아이템 1개 획득
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
