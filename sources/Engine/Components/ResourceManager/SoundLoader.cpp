#define DR_WAV_IMPLEMENTATION
#include <Engine/Components/Sound/dr_wav.h>

#include "SoundLoader.h"
SoundLoader::SoundLoader()
{
}

SoundLoader::~SoundLoader()
{
}

BaseResourceInstance * SoundLoader::load(const std::string & path, std::optional<std::any> options)
{
	drwav* pWav = drwav_open_file(path.c_str());

	if (pWav == NULL) {
		return nullptr;
	}

	unsigned long long audioFileSize = (unsigned long long)pWav->totalPCMFrameCount * pWav->channels * sizeof(int16_t);

	std::byte* pSampleData = new std::byte[audioFileSize];
	drwav_read_pcm_frames_s16(pWav, pWav->totalPCMFrameCount, (drwav_int16*)pSampleData);
	unsigned int sampleRate = pWav->sampleRate;

	Sound* loaded = new Sound(pSampleData, (size_t)pWav->channels, audioFileSize, sampleRate);
	drwav_close(pWav);
	delete pSampleData;

	return new ResourceInstance<Sound>(loaded);
}
