#include "pch.h"
#include "Sound.h"


using namespace std;

void Sound::update()
{
}

Sound::Sound(vector<string>& fileNames)
{
	m_audioEngine = DirectX::AudioEngine();
	m_componentType = ComponentEnum::SOUND;

	std::wstring ws;
	for (int i = 0; i < fileNames.size(); ++i)
	{
		ws = std::wstring(fileNames[i].begin(), fileNames[i].end());
		m_sounds.emplace_back(&m_audioEngine, ws.c_str());
		m_map[fileNames[i]] = i;
	}
}

Sound::~Sound()
{
	m_audioEngine.Suspend();
}

size_t Sound::play()
{
	m_sounds[0].Play();
	return m_sounds[0].GetSampleDuration();
}
