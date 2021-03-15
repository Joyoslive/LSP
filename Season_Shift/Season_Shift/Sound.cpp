#include "pch.h"
#include "Sound.h"
#include "Transform.h"
#include "CameraComponent.h"


using namespace std;
HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

Sound::Sound()
{

}

Sound::Sound(const vector<string>& fileNames)
{
	m_loopingSoundId = -1;
	m_componentType = ComponentEnum::SOUND;
	m_audioEngine = std::make_unique<DirectX::AudioEngine>();

	std::wstring ws;
	for (int i = 0; i < fileNames.size(); ++i)
	{
		ws = std::wstring(fileNames[i].begin(), fileNames[i].end());
		m_sounds.emplace_back(m_audioEngine.get(), ws.c_str());
		m_map[fileNames[i]] = i;
	}
}

Sound::~Sound()
{
	if (m_effectInst != nullptr) m_effectInst.reset();
	m_audioEngine->Suspend();
}

void Sound::start(const vector<string>& fileNames)
{
	m_loopingSoundId = -1;
	m_componentType = ComponentEnum::SOUND;
	m_audioEngine = std::make_unique<DirectX::AudioEngine>();

	std::wstring ws;
	for (int i = 0; i < fileNames.size(); ++i)
	{
		ws = std::wstring(fileNames[i].begin(), fileNames[i].end());
		m_sounds.emplace_back(m_audioEngine.get(), ws.c_str());
		m_map[fileNames[i]] = i;
	}
}

void Sound::update()
{
	m_audioEngine->Update();
}

void Sound::initialize()
{
	update();
}



size_t Sound::play(const std::string& soundName)
{
	
	m_sounds[m_map[soundName]].Play();
	return m_sounds[m_map[soundName]].GetSampleDuration();
}


void Sound::playLoop(const std::string& soundName)
{
	if (m_effectInst != nullptr)
	{
		if (m_effectInst->IsLooped() && m_map[soundName] != m_loopingSoundId)
		{
			m_effectInst->Stop(true);
		}
	}
	m_effectInst = m_sounds[m_map[soundName]].CreateInstance();
	m_effectInst->Play(true);
	m_loopingSoundId = m_map[soundName];
}

void Sound::stopLoop()
{
	if (m_effectInst == nullptr) return;
	if(m_effectInst->IsLooped())
		m_effectInst->Stop(true);
}

void Sound::setVolume(float volume)
{
	m_audioEngine->SetMasterVolume(volume);
}

void Sound::setPitch(float pitch)
{
	if(m_effectInst)
		m_effectInst->SetPitch(pitch);
}