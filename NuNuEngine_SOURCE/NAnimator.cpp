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
		for (auto& it : mAnimations)
		{
			delete it.second;
			it.second = nullptr;
		}

		for (auto& it : mEvents)
		{
			delete it.second;
			it.second = nullptr;
		}
	}

	void Animator::Initialize()
	{
	}

	void Animator::Update()
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Update();

			Events* events = FindEvents(mActiveAnimation->GetName());

			if (mActiveAnimation->IsComplete() == true)
			{
				if (events) 
					events->CompleteEvent();

				if (mbLoop == true) mActiveAnimation->Reset();
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
		anim->SetName(name);
		anim->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration);
		anim->SetAnimator(this);

		Events* events = new Events();
		mEvents.insert(std::make_pair(name, events));

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

		if (mActiveAnimation)
		{
			Events* curEvents = FindEvents(mActiveAnimation->GetName());
			if (curEvents) curEvents->EndEvent();
		}

		Events* nextEvents = FindEvents(anim->GetName());
		if (nextEvents)	nextEvents->StartEvent();

		mActiveAnimation = anim;
		mActiveAnimation->Reset();
		mbLoop = loop;
	}

	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		auto it = mEvents.find(name);

		if (it == mEvents.end()) return nullptr;

		return it->second;
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->StartEvent.mEvent;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->CompleteEvent.mEvent;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->EndEvent.mEvent;
	}
}