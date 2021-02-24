#include "Sound.h"


void Sound::update()
{
}

Sound::Sound()
{
	m_audioEngine = DirectX::AudioEngine();
	m_sound = std::make_shared<DirectX::SoundEffect>(&m_audioEngine, L"Sounds/Explo4.wav");
	m_componentType = ComponentEnum::SOUND;
}

Sound::~Sound()
{
	m_audioEngine.Suspend();
}

void Sound::play()
{
	m_sound->Play();
}
