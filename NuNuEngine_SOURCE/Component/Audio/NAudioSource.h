#pragma once
#include "../Component/NComponent.h"
#include "Resource/Audio/NAudioClip.h"

namespace NuNu
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void Play();
		void Stop();
		void SetLoop(bool loop);

		void SetClip(AudioClip* clip) { mAudioClip = clip; }
		AudioClip* GetClip() { return mAudioClip; }

	private:
		AudioClip* mAudioClip;
	};
}


