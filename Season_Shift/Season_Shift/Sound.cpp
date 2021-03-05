#include "pch.h"
#include "Sound.h"
#include "Transform.h"
#include "CameraComponent.h"


using namespace std;
HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);


Sound::Sound(const vector<string>& fileNames, const Ref<CameraComponent>& listener)
{
	m_componentType = ComponentEnum::SOUND;
	m_listenerCamera = listener;
	if (listener != nullptr)
	{
		m_audioEngine = std::make_unique<DirectX::AudioEngine>(DirectX::AudioEngine_EnvironmentalReverb 
			| DirectX::AudioEngine_ReverbUseFilters
			| DirectX::AudioEngine_UseMasteringLimiter
			| DirectX::AudioEngine_DisableVoiceReuse); //this may work,
		m_emitter = DirectX::AudioEmitter();
		m_listener = DirectX::AudioListener();
	}
	else
	{
		m_audioEngine = std::make_unique<DirectX::AudioEngine>();
	}

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

void Sound::update()
{
	m_audioEngine->Update();
	if (m_listenerCamera != nullptr)
	{
		m_emitter.SetPosition(getTransform()->getPosition());
		m_emitter.SetOrientation(getTransform()->getWorldMatrix().Forward(), getTransform()->getWorldMatrix().Up());

		m_listener.SetPosition(m_listenerCamera->getCamera()->getPosition());
		m_listener.SetOrientation(m_listenerCamera->getCamera()->getForward(), m_listenerCamera->getCamera()->getUp());
	}
}

void Sound::initialize()
{
	update();
}



size_t Sound::play(const std::string& soundName)
{
	if (m_listenerCamera != nullptr)
	{
		m_effectInst = m_sounds[m_map[soundName]].CreateInstance(DirectX::SoundEffectInstance_Use3D);
		m_effectInst->Play(true);
		m_effectInst->Apply3D(m_listener, m_emitter, false);
	}
	else
	{
		/*m_effectInst = m_sounds[m_map[soundName]].CreateInstance();
		m_effectInst->Play();*/
		m_sounds[m_map[soundName]].Play();
	}
	return m_sounds[m_map[soundName]].GetSampleDuration();


}

void Sound::playLoop(const std::string& soundName)
{
	if (m_effectInst != nullptr)
	{
		if (m_effectInst->IsLooped())
		{
			m_effectInst->Stop(true);
		}
	}
	m_effectInst = m_sounds[m_map[soundName]].CreateInstance();
	m_effectInst->Play(true);
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
