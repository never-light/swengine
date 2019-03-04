#pragma once

#include <vector>
#include <Windows.h>
#include <Engine/Components/Sound/Sound.h>
#include <Engine/Components/Math/types.h>

class SoundSource
{
public:
	SoundSource();
	SoundSource(const vector3 & position, const vector3 & velocity, bool relative,
		float gain = 1.0, float pitch = 1.0,bool looped = false);
	~SoundSource();

	void setPosition(const vector3 & position);
	void setVelocity(const vector3 & velocity);
	void setRelative(bool relative);
	void setGain(float gain);
	void setPitch(float pitch);
	void isLooped(bool looped);

	vector3 &  getPosition();
	vector3 & getVelocity();
	float getGain();
	float getPitch();
	bool getLooped();

	void bindSound(Sound* sound);
	void play();
	void pause();
	void pauseOn(ALuint pauseTime);
	void stop();
	void setSpeed(ALfloat speed);
	bool isPaused();
	bool isPlaying();
	unsigned int getSourceID();


	// getters and setters for every field this class;

private:
	
	vector3 m_position;
	vector3 m_velocity;
    bool m_relative;
	float m_gain;
	float m_pitch;
	bool m_looped;

	Sound* sound;
	unsigned int sourceID;
};
