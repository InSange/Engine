#pragma once
#include "../Component/NComponent.h"
#include "Resource/Audio/NAudioClip.h"

namespace NuNu
{
	class AudioSource final : public Component
	{
	public:
		AudioSource();
		virtual ~AudioSource();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void Play() const;
		void Stop() const;
		void SetLoop(bool loop) const;

		void SetClip(AudioClip* clip) { mAudioClip = clip; }
		AudioClip* GetClip() const { return mAudioClip; }

	private:
		AudioClip* mAudioClip;
	};
}


