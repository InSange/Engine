#pragma once
#include "Fmod/NFmod.h"
#include "Resource/NResource.h"

namespace NuNu
{
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		HRESULT Load(const std::wstring& path) override;

		void Play();
		void Stop();
		void Set3DAttributes(Vector3 pos);

		void SetLoop(bool loop) { mbLoop = loop; }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;

		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}


