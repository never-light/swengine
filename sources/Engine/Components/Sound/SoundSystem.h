#pragma once

#include <OpenAL/alc.h>

#include <Engine/Components/Sound/SoundListener.h>
#include <Engine/Components/Sound/Sound.h>
#include <Engine/Components/ResourceManager/SoundLoader.h>
#include <Engine/Components/Sound/SoundSource.h>

class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();

	void update(float delta);

	SoundListener* getCurrentSoundListener() const;

private:
	ALCdevice* m_alDevice;
	ALCcontext* m_alContext;

	SoundListener* m_currentSoundListener;
};