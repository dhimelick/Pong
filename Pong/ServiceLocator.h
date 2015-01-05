#pragma once

#include "stdafx.h"
#include "IAudioProvider.h"

class ServiceLocator
{
public:
	static IAudioProvider* getAudio()  { return audioProvider; } const

		static void registerServiceLocator(IAudioProvider *provider)
	{
		audioProvider = provider;
	}

private:
	static IAudioProvider * audioProvider;
};

