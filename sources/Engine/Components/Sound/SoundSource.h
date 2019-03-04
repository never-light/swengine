#pragma once

#include <vector>

#include <Engine/Components/Sound/Sound.h>
#include <Engine/Components/Math/types.h>

class SoundSource {
public:
	SoundSource();
	SoundSource(const vector3 & position, const vector3 & velocity, bool relative,
		float gain = 1.0, float pitch = 1.0,bool looped = false);

	SoundSource(SoundSource&& source);

	~SoundSource();

	void setPosition(const vector3 & position);
	void setVelocity(const vector3 & velocity);
	void setRelative(bool relative);
	void setGain(float gain);
	void setPitch(float pitch);
	void setLooped(bool looped);

	vector3 getPosition() const;
	vector3 getVelocity() const;
	float getGain() const;
	float getPitch() const;
	bool isLooped() const;

	void bindSound(Sound* sound);
	void play();
	void pause();
	void pauseOn(ALuint pauseTime);
	void stop();
	void setSpeed(ALfloat speed);
	bool isPaused() const;
	bool isPlaying() const;
	unsigned int getSourceID() const;

private:
	void initializeALSoundSource();

private:
	SoundSource(const SoundSource& source) = delete;
	SoundSource& operator=(const SoundSource&) = delete;
private:
	vector3 m_position;
	vector3 m_velocity;
    bool m_relative;
	float m_gain;
	float m_pitch;
	bool m_looped;

	Sound* m_sound;
	unsigned int m_sourceID;
};
