#include "Fmod/NFmod.h"

namespace NuNu
{
	FMOD::Studio::System* Fmod::mSystem = nullptr;
	FMOD::System* Fmod::mCoreSystem = nullptr;

	void Fmod::Initialize()
	{
		void* extraDriverData = nullptr;

		FMOD::Studio::System::create(&mSystem);

		mSystem->getCoreSystem(&mCoreSystem);
		mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

		mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

/*		FMOD::Studio::Bank* masterBank = nullptr;
		FMOD::Sound* sound1 = nullptr; // 반드시 초기화 필요!!
		FMOD_RESULT result;
		FMOD::Channel* channel1 = nullptr;
		
		result = mCoreSystem->createSound("..\\Resources\\Sound\\smw_bonus_game_end.wav", FMOD_3D, 0, &sound1);

		// 파일 로드에 성공(FMOD_OK)해서 sound1이 무사히 생성되었을 때만 처리하도록 안전장치 추가!
		if (result == FMOD_OK && sound1 != nullptr)
		{
			const float DISTANCEFACTOR = 1.0f;
			result = sound1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
			result = sound1->setMode(FMOD_LOOP_OFF);

			FMOD_VECTOR pos = { 0.0f * DISTANCEFACTOR, 0.0f, 10.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			result = mCoreSystem->playSound(sound1, 0, false, &channel1);
			if (result == FMOD_OK && channel1 != nullptr)
			{
				result = channel1->set3DAttributes(&pos, &vel);
			}

			static float t = 0;
			static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
			FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
			FMOD_VECTOR vel2 = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
			result = mCoreSystem->set3DListenerAttributes(0, &listenerpos, &vel2, &forward, &up);

			SoundPlay(sound1, &channel1);
		}*/
	}

	bool Fmod::CreateSound(const std::string& path, FMOD::Sound** sound)
	{
		if (FMOD_OK != mCoreSystem->createSound(path.c_str(), FMOD_3D, 0, sound))
			return false;

		return true;
	}

	void Fmod::SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel)
	{
		mCoreSystem->playSound(sound, 0, false, channel);
	}

	void Fmod::Set3DListenerAttributes(const Vector3& pos)
	{
		FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
		FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);
		FMOD_VECTOR fmodForward(0.0f, 0.0f, 1.0f);
		FMOD_VECTOR fmodUp(0.0f, 1.0f, 0.0f);

		mCoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
	}

	void Fmod::Release()
	{
		mSystem->release();
		mSystem = nullptr;
	}
}
