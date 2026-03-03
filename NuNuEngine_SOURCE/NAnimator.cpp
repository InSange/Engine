#include "NAnimator.h"

namespace NuNu
{
	Animator::Animator()
		: Component(enums::eComponentType::Animator)
		, mAnimations{}
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{
	}
	Animator::~Animator()
	{
	}
	void Animator::Initialize()
	{
	}
	void Animator::Update()
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Update();
			if (mActiveAnimation->IsComplete() == true && mbLoop == true)
			{
				mActiveAnimation->Reset();
			}
		}
	}
	void Animator::LateUpdate()
	{
	}
	void Animator::Render(HDC hdc)
	{
		if (mActiveAnimation)
			mActiveAnimation->Render(hdc);
	}
	void Animator::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		Animation* anim = nullptr;

		anim = FindAnimation(name);
		if (anim != nullptr) return;

		anim = new Animation();
		anim->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration);
		anim->SetAnimator(this);

		mAnimations.insert(std::make_pair(name, anim));
	}
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		auto it = mAnimations.find(name);

		if (it == mAnimations.end()) return nullptr;

		return it->second;
	}
	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* anim = FindAnimation(name);

		if (anim == nullptr) return;

		mActiveAnimation = anim;
		mActiveAnimation->Reset();
		mbLoop = loop;
	}
}