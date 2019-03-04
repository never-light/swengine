#pragma once

#include <Engine/Components/AL/alc.h>
#include <Engine/Components/Sound/SoundListener.h>
#include<Engine/Components/Sound/Sound.h>
#include <Engine/Components/ResourceManager/SoundLoader.h>
#include <Engine/Components/Sound/SoundSource.h>

void initializateOpenAL()
{
	ALCdevice* pDevice = alcOpenDevice(NULL);
	ALCcontext*pContext = alcCreateContext(pDevice, NULL);
	alcMakeContextCurrent(pContext);
}