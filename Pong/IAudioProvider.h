#pragma once

#include "stdafx.h"

class IAudioProvider
{
public:
	virtual ~IAudioProvider() {}
	virtual void playSound(std::string filename) = 0;
	virtual void playSong(std::string filename, bool looping) = 0;
	virtual void stopAllSounds() = 0;

	virtual bool isSoundPlaying() = 0;
	virtual bool isSongPlaying() = 0;
};
