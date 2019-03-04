#include "SoundLoader.h"

#include "ResourceLoadingException.h"

#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>

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
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "File loading error", __FILE__, __LINE__, __FUNCTION__);
	}

	if (pWav->channels > 2) {
		drwav_close(pWav);
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "The sound file has more than 2 audio channels", __FILE__, __LINE__, __FUNCTION__);
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
