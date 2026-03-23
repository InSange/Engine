#include "Component/Audio/NAudioSource.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Fmod/NFmod.h"

namespace NuNu
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)\
		, mAudioClip(nullptr)
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play() const
	{
		mAudioClip->Play();
	}

	void AudioSource::Stop() const
	{
		mAudioClip->Stop();
	}

	void AudioSource::SetLoop(bool loop) const
	{
		mAudioClip->SetLoop(loop);
	}
}
