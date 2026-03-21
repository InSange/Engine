#include "Component/Audio/NAudioListener.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Fmod/NFmod.h"

namespace NuNu
{
	AudioListener::AudioListener()
		: Component(eComponentType::AudioListener)
	{
	}

	AudioListener::~AudioListener()
	{
	}

	void AudioListener::Initialize()
	{
	}

	void AudioListener::Update()
	{
	}

	void AudioListener::LateUpdate()
	{
		Transform* tr = nullptr;
		tr = GetOwner()->GetComponent<Transform>();
		
		Vector3 pos;
		Fmod::Set3DListenerAttributes(pos);
	}

	void AudioListener::Render()
	{
	}
}
