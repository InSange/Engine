#include "Component/Animator/NAnimator.h"
#include "Resource/NResources.h"
#include "Resource/Texture/NTexture.h"

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

	void Animator::CreateAnimationByFoler(const std::wstring& name, const std::wstring& path, Vector2 offset, float duration)
	{
		Animation* anim = nullptr;

		anim = FindAnimation(name);
		if (anim != nullptr) return;

		int fileCount = 0;
		std::filesystem::path fs(path);
		std::vector<graphics::Texture*> images = {};

		for (auto& p : std::filesystem::recursive_directory_iterator(fs))
		{
			std::wstring fileName = p.path().filename();
			std::wstring fullName = p.path();

			graphics::Texture* texture = Resources::Load<graphics::Texture>(fileName, fullName);
			images.push_back(texture);
			fileCount++;
		}

		if (images.empty()) return;
		std::wstring ext = L"";
		if (images[0]->GetTextureType() == graphics::Texture::eTextureType::Bmp)
			ext = L".bmp";
		else if (images[0]->GetTextureType() == graphics::Texture::eTextureType::Png)
			ext = L".png";
		else if (images[0]->GetTextureType() == graphics::Texture::eTextureType::jpg)
			ext = L".jpg";

		std::wstring sheetName = name + ext;

		UINT sheetWidth = images[0]->GetWidth() * fileCount;
		UINT sheetHeight = images[0]->GetHeight();
		graphics::Texture* spriteSheet = graphics::Texture::Create(sheetName, sheetWidth, sheetHeight);

		UINT imageWidth = images[0]->GetWidth();
		UINT imageHeight = images[0]->GetHeight();
		if (spriteSheet->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		{
			for (size_t i = 0; i < images.size(); i++)
			{
				BitBlt(spriteSheet->GetHdc(), i * imageWidth, 0, imageWidth, imageHeight,
					images[i]->GetHdc(), 0, 0, SRCCOPY);
			}
		}
		else if (spriteSheet->GetTextureType() == graphics::Texture::eTextureType::Png ||
			spriteSheet->GetTextureType() == graphics::Texture::eTextureType::jpg)
		{
			Gdiplus::Graphics graphics(spriteSheet->GetImage());

			for (size_t i = 0; i < images.size(); i++)
			{
				Gdiplus::Image* frameImage = images[i]->GetImage();
				if (frameImage != nullptr)
				{
					graphics.DrawImage(frameImage,
						static_cast<INT>(i * imageWidth), 0,
						static_cast<INT>(imageWidth), static_cast<INT>(imageHeight));
				}
			}
		}

		CreateAnimation(name, spriteSheet, Vector2::Zero, Vector2(imageWidth, imageHeight), offset, fileCount, duration);
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

