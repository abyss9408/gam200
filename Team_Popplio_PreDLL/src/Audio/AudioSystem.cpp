/******************************************************************************/
/*!
\file   AudioSystem.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 98% Val - 2%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the source file for the Audio System (uses FMOD integration)

		Implements FMOD interface
		Enables manipulation of audio files
		Does FFT representation
		Reads / writes to MIDI files (TODO)

		Reference:
		https://fmod.com/docs/2.03/api/welcome.html

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include <fmod_common.h>

#include "AudioSystem.h"

#include "AudioComponent.h"
#include "../AssetStore/AssetStore.h"

#include "FFT.h"

#include "../Logging/Logger.h"
#include "../Engine/Engine.h"

#include <stdexcept>
#include <limits>

namespace Popplio
{

	// Class Audio

	bool Audio::GetPlaying()
	{
		bool isPlaying;
		rSystem.SetResult(UseChannel()->isPlaying(&isPlaying));
		return isPlaying;
	}

	bool Audio::GetPaused()
	{
        bool isPaused;
		rSystem.SetResult(UseChannel()->getPaused(&isPaused));
		return isPaused;
	}

	bool Audio::GetStopped()
	{
		return !GetPlaying();
	}

	Audio::Audio(AudioSystem& sys, std::string audioID, FMOD::ChannelGroup* grp, size_t const& i,
		bool const& instanced, float const& beatsPM, std::pair<int, int> const& timeS)
        : rSystem(sys), audioID(audioID), pChannel(), group(grp), index(i), isInstance(instanced), 
		volume(1.f), mute(false), loop(false), playOnAwake(false), groupIndex(-1), samplerate(-1.f)
		//, isPlaying(false), isPaused(false)
	{
		if (audioID != AUDIO_DEFAULT)
		{
			if (AssetStore::GetAudio(audioID) == nullptr) // error out if audio not found
			{
				rSystem.SetResult(FMOD_ERR_FILE_BAD);
				Logger::Error("AudioSystem: Audio File [" + audioID + "] not found", LogDestination::IMGUI_CONSOLE);
				//throw std::runtime_error("AudioSystem: Audio File [" + audioID + "] not found");
			}

			//rSystem.SetResult(AssetStore::GetAudio(audioID)->getLength(&(len), defaultUnit));

			SetBPM(beatsPM);
			AssetStore::GetAudio(audioID)->getDefaults(&samplerate, nullptr);

			for (int h = 0; h <= POPPLIO_AUDIO_FFT_BIN_COUNT; ++h) 
			{
				float binEdge = POPPLIO_AUDIO_FFT_MIN_FREQ * powf(GetSampleRate() / 2.f / POPPLIO_AUDIO_FFT_MIN_FREQ,
					static_cast<float>(h) / POPPLIO_AUDIO_FFT_BIN_COUNT);
				logBins.push_back(binEdge);
			}

            AssetStore::GetAudio(audioID)->setMusicSpeed(1.f);
		}
		else
		{
			bpm = beatsPM;
			beatInterval = 0.f;
		}

		timeSig = timeS;

		isInThreshold = false;

		if (group != nullptr)
		{
			for (size_t j{}; j < rSystem.GetCGroup().size(); ++j)
			{
                if (rSystem.GetCGroup(j) == group)
                {
                    groupIndex = static_cast<int>(j);
                    break;
                }
			}
            if (groupIndex == -1)
            {
                Logger::Error("Audio: ChannelGroup not found");
                //throw std::runtime_error("Audio: ChannelGroup not found");
            }
		}
		else
		{
            groupIndex = -1;
			Logger::Info("Audio: Null ChannelGroup");
			//throw std::runtime_error("Audio: Null ChannelGroup");
		}
	}

	Audio::Audio(Audio const& rhs) : 
		rSystem(rhs.rSystem), audioID(rhs.audioID), pChannel(rhs.pChannel), group(rhs.group), index(rhs.index), 
		isInstance(rhs.isInstance), 
        volume(rhs.volume), mute(rhs.mute), loop(rhs.loop), timeSig(rhs.timeSig), playOnAwake(rhs.playOnAwake), 
		groupIndex(rhs.groupIndex), samplerate(rhs.samplerate), logBins(rhs.logBins)
		//, isPlaying(rhs.isPlaying), isPaused(rhs.isPaused)
	{
		if (AssetStore::GetAudio(audioID) == nullptr) // error out if audio not found
		{
			rSystem.SetResult(FMOD_ERR_FILE_BAD);
			Logger::Error("AudioSystem: Audio File [" + audioID + "] not found", LogDestination::IMGUI_CONSOLE);
			throw std::runtime_error("AudioSystem: Audio File [" + audioID + "] not found");
		}

		//pChannel.emplace(*rhs.pChannel);
		//group.emplace(*rhs.group);

		//rSystem.SetResult(AssetStore::GetAudio(audioID)->getLength(&(len), defaultUnit));

		SetBPM(rhs.bpm);

		isInThreshold = false;
	}

	Audio& Audio::operator=(Audio const& rhs) 
	{
		// ignore system

		audioID = rhs.audioID;

		// not copied (same pointers)
		pChannel = rhs.pChannel;
		group = rhs.group;

		index = rhs.index;
		volume = rhs.volume;
        mute = rhs.mute;
		loop = rhs.loop;

		bpm = rhs.bpm;
		beatInterval = rhs.beatInterval;

		timeSig = rhs.timeSig;

		isInThreshold = false;

		//isPlaying = false;
		//isPaused = false;

		playOnAwake = rhs.playOnAwake;
		groupIndex = rhs.groupIndex;

		samplerate = rhs.samplerate;
        logBins = rhs.logBins;

        isInstance = rhs.isInstance; // error?

		return *this;
	}

    bool Audio::operator==(Audio& rhs)
    {
		return this->UseSound() == rhs.UseSound();
    }

	//------------------

	//FMOD::Sound* Audio::useAudio() { return Audio::pAudio; }
	FMOD::Sound* Audio::UseSound() { return AssetStore::GetAudio(audioID); }
	FMOD::Channel* Audio::UseChannel() { return std::addressof(*pChannel); }

	FMOD::ChannelGroup* Audio::GetGroup() const { return group; }
	void Audio::SetGroup(FMOD::ChannelGroup* grp)
	{
		group = grp;
		for (size_t i{}; i < rSystem.GetCGroup().size(); ++i)
		{
			if (rSystem.GetCGroup(i) == group)
			{
				groupIndex = static_cast<int>(i);
				break;
			}
		}
		//if (group != std::nullopt) group.reset();
		//group.emplace(grp); 
	}

	int Audio::GetGroupByIndex() const
	{
		return groupIndex;
	}
	void Audio::SetGroupByIndex(int const& grp)
	{
		groupIndex = grp;
        group = rSystem.GetCGroup(grp);
	}

	//bool const Audio::GetPlaying() 
	//{
	//	bool temp;
	//	Audio::rSystem.SetResult(pChannel->isPlaying(&temp));
	//	return temp;
	//}
	size_t const& Audio::GetIndex() const { return index; }
    void Audio::SetIndex(size_t const& i) { index = i; }

	float const& Audio::GetVolume() const { return volume; }
    float const& Audio::SetVolume(float const& vol) 
	{ 
        if (!GetPlaying()) return volume = vol;
		if (!mute)
		{
			rSystem.SetResult(pChannel->setVolume(vol));
			rSystem.ErrorCheck();
		}
		return volume = vol; 
	}

	bool const& Audio::GetMute() const { return mute; }
	bool const& Audio::SetMute(bool const& m)
	{
        if (!GetPlaying()) return mute = m;
        if (m)
        {
            rSystem.SetResult(pChannel->setVolume(0.f));
            rSystem.ErrorCheck();
        }
        else
        {
            rSystem.SetResult(pChannel->setVolume(volume));
            rSystem.ErrorCheck();
        }

		return mute = m;
	}

    bool const& Audio::GetLoop()
	{ 
		FMOD_MODE* mode = nullptr;
		pChannel->getMode(mode);
		if (mode != nullptr) loop = *mode & FMOD_LOOP_NORMAL;
		//pChannel->getMode(&mode);
		//loop = (mode == FMOD_LOOP_NORMAL);
		return loop; 
	}
	bool const& Audio::SetLoop(bool const& doLoop)
	{
        loop = doLoop;
		UseChannel()->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
        UseChannel()->setLoopCount(loop ? -1 : 0);
		return loop;
	}

	unsigned int Audio::GetLen() const 
	{ 
		unsigned int len{ std::numeric_limits<unsigned int>().max() };
		rSystem.SetResult(AssetStore::GetAudio(audioID)->getLength(&(len), defaultUnit)); 
        return len;
	}

    float const& Audio::GetBPM() const { return bpm; }

	float const& Audio::SetBPM(float const& b)
	{
        bpm = b;
		beatInterval =
			static_cast<float>(GetLen()) / (bpm * (static_cast<float>(GetLen()) / (60 * 1000)));
        return bpm;
	}

    float const& Audio::GetBeatInterval() const { return beatInterval; }

	std::pair<int, int> const& Audio::SetTimeSig(std::pair<int, int> const& ts)
    {
        timeSig = ts;
        return timeSig;
    }

    std::pair<int, int> const& Audio::GetTimeSig() const { return timeSig; }

	std::string const& Audio::GetID() const { return audioID; }

	std::string const& Audio::SetID(std::string const& s)
	{
        audioID = s;
        return audioID;
	}

	AudioSystem& Audio::UseSys() { return rSystem; }
	AudioSystem const& Audio::GetSys() const { return rSystem; }

	float const& Audio::GetSampleRate()
	{
        if (samplerate == -1.f)
        {
            AssetStore::GetAudio(audioID)->getDefaults(&samplerate, nullptr);
        }

		return samplerate;
	}

	//void Audio::SetIndex(size_t const& i) { Audio::index = i; }

	//------------------

	//float const& Audio::GetBPM() const { return Audio::bpm; }
	//std::pair<int, int> const& Audio::GetTimeSig() const { return Audio::timeSig; }

	//unsigned int const& Audio::GetLen() const { return Audio::len; }

	unsigned int const Audio::GetCurr() 
	{
		//rSystem.Update(); // try
		unsigned int pos{};
		Audio::pChannel->getPosition(&pos, Audio::defaultUnit);
		return pos;
	}
	void Audio::SetCurr(unsigned int const& pos)
	{
		Audio::pChannel->setPosition(pos, Audio::defaultUnit);
	}

	bool const Audio::IsOnBPM()
	{
		if (Audio::bpm <= 0) return false;

		// Expensive ?
		if (!isInThreshold && Audio::beatThreshold > Audio::GetCurr() % static_cast<unsigned int>(Audio::beatInterval))
		{
			isInThreshold = true;
			return true;
		}

		if (isInThreshold && Audio::beatThreshold <= Audio::GetCurr() % static_cast<unsigned int>(Audio::beatInterval))
			isInThreshold = false;

		return false;
	}

	//------------------

	FMOD_RESULT const& Audio::Play()
	{
		if (GetPlaying() && !GetPaused()) return rSystem.GetResult();

		rSystem.SetResult
		(
			GetPlaying() ?
			pChannel->setPaused(false) :
			rSystem.UseSys()->playSound(AssetStore::GetAudio(audioID), GetGroup(), false, &pChannel)
		);

		//rSystem.SetPlaying(isPlaying = true);

		//isPaused = false;

		//if (rSystem.GetResult() == FMOD_OK)
		//{
		//	rSystem.SetPlaying(isPlaying = true);

		//	isPaused = false;
		//}

        if (!mute) rSystem.SetResult(pChannel->setVolume(volume));
        else rSystem.SetResult(pChannel->setVolume(0.f));
        rSystem.ErrorCheck();

		return rSystem.GetResult();
	}

	FMOD_RESULT const& Audio::Pause()
	{
		if (GetPaused()) return rSystem.GetResult();

		rSystem.SetResult
		(
			pChannel->setPaused(true)
		);

		//isPaused = true;
		//isPlaying = false;

		//if (rSystem.GetResult() == FMOD_OK)
		//{
		//	isPaused = true;
		//	isPlaying = false;
		//}

		return rSystem.GetResult();
	}

	FMOD_RESULT const& Audio::Stop()
	{
		if (!GetPlaying()) return rSystem.GetResult();

		rSystem.SetResult
		(
			pChannel->stop()
		);

		//isPaused = false;
		//isPlaying = false;

		//if (rSystem.GetResult() == FMOD_OK)
		//{
		//	isPaused = false;
		//	isPlaying = false;
		//}

		return rSystem.GetResult();
	}

	int const Audio::OffBeat() 
	{
		int off{ static_cast<int>(GetCurr() % static_cast<unsigned int>(Audio::beatInterval)) };
		return off < static_cast<int>(Audio::beatInterval / 2.f) ?
			off : off - static_cast<int>(Audio::beatInterval);
	}

	bool const Audio::GetSpectrum(float** fft, float magMod) 
	{
		bool bypass{};
		UseSys().SetResult(UseSys().fft->getBypass(&bypass));
		UseSys().ErrorCheck();

        if (bypass) UseSys().fft->setBypass(false);

		FMOD_DSP_PARAMETER_FFT *data = nullptr;

		UseSys().SetResult(UseSys().fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&data, 0, 0, 2));

		if (data == nullptr) return false;

        if (data->length == 0 || data->numchannels == 0) return false; // prevent null data, as during boot up it may be null

		// ---

		//for (size_t channel{}; channel < 2; channel++)
		//	for (size_t bin{}; bin < POPPLIO_AUDIO_FFT_BIN_COUNT; ++bin)
		//		fft[channel][bin] = data->spectrum[channel][bin] * magMod;

		// ===

		float nyquistFreq = GetSampleRate() / 2.f;
		float binWidth = nyquistFreq / POPPLIO_AUDIO_FFT_SAMPLE_SIZE;

		for (int channel{}; channel < 2; ++channel)
		{
			// Aggregate FFT magnitudes into linear bins
			for (int i = 0; i < POPPLIO_AUDIO_FFT_SAMPLE_SIZE; ++i) 
			{
				float freq = (i * nyquistFreq) / POPPLIO_AUDIO_FFT_SAMPLE_SIZE; // Frequency corresponding to this FFT bin
				int binIndex = static_cast<int>(freq / binWidth);

				if (binIndex < POPPLIO_AUDIO_FFT_BIN_COUNT) 
				{
					fft[channel][binIndex] += data->spectrum[channel][i] * magMod; // Accumulate magnitude
				}
			}
		}

		// ---

		if (UseSys().ErrorCheck()) return true;
		else return false;
	}

	bool const Audio::GetSpectrumLog(float** fft, float magMod)
	{
		bool bypass{};
		UseSys().SetResult(UseSys().fft->getBypass(&bypass));
		UseSys().ErrorCheck();

		if (bypass) UseSys().fft->setBypass(false);

		FMOD_DSP_PARAMETER_FFT* data = nullptr;

		UseSys().SetResult(UseSys().fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&data, 0, 0, 2));

		if (data == nullptr) return false;

		if (data->length == 0 || data->numchannels == 0) return false; // prevent null data, as during boot up it may be null

		// map

		float nyquistFreq = GetSampleRate() / 2.f; 
		float binWidth = nyquistFreq / POPPLIO_AUDIO_FFT_SAMPLE_SIZE;

		for (int channel{}; channel < 2; ++channel)
		{
			for (int i = 0; i < POPPLIO_AUDIO_FFT_BIN_COUNT; ++i)
			{
				float binStart = logBins[i];
				float binEnd = logBins[i + 1];

				// find FFT indices corresponding to the current logarithmic bin
				int startIdx = static_cast<int>(binStart / binWidth);
				int endIdx = static_cast<int>(binEnd / binWidth);

				float binSum = 0.0f;
				int count = 0;

				for (int j = startIdx; j < endIdx && j < POPPLIO_AUDIO_FFT_SAMPLE_SIZE; ++j)
				{
					binSum += data->spectrum[channel][j];
					++count;
				}

				//fft[channel][i] = (count > 0) ? binSum / count * magMod : 0.0f; // Average the values
				if (count > 0)
				{
					fft[channel][i] = binSum / count * magMod;
					for (int k = i - 1; k >= 0; --k)
					{
						if (fft[channel][k] <= 0.f) std::swap(fft[channel][k], fft[channel][k + 1]);
						else break;
					}
				}
				else fft[channel][i] = 0.f;
			}
		}

		if (UseSys().ErrorCheck()) return true;
		else return false;
	}

	void Audio::Kill()
	{
		if (!isInstance)
		{
			Logger::Warning("Audio | Attempted to kill non-instanced Audio");
		}
		else
		{
            UseSys().RemoveAudioInst(index);
		}
	}

	// ------------------------------------------------------------------------------------- //

	// Class AudioSystem

	AudioSystem::AudioSystem(Registry& r) : reg(r), pFSystem(nullptr), fResult(FMOD_OK), version(0),
		audio(), inst(), maxChannels(POPPLIO_AUDIO_MAX_CHANNELS), fMode(FMOD_DEFAULT), isPlaying(false), 
		//entitiesWithAudio(),
		defaultAudio(*this, AUDIO_DEFAULT, nullptr, std::numeric_limits<size_t>().max(), false)
	{
		RequireComponent<AudioComponent>(); // only let audio components interact

		SetResult(FMOD::System_Create(&pFSystem));
		ErrorCheck();
		pFSystem->init(static_cast<int>(maxChannels), FMOD_INIT_NORMAL, nullptr);

		//SetResult(pFSystem->getVersion(&version));
		//ErrorCheck();

		//if (version < FMOD_VERSION) Logger::Error("FMOD lib version doesn't match header version");


		// create mst (master), sfx (sound), bgm (music) channel groups
		FMOD::ChannelGroup* mst{ nullptr };
		FMOD::ChannelGroup* sfx{ nullptr };
		FMOD::ChannelGroup* bgm{ nullptr };

		//SetResult(pFSystem->createChannelGroup("mst", &mst));
		SetResult(pFSystem->getMasterChannelGroup(&mst));
		ErrorCheck();
		SetResult(pFSystem->createChannelGroup("sfx", &sfx));
		ErrorCheck();
		SetResult(pFSystem->createChannelGroup("bgm", &bgm));
		ErrorCheck();

		SetResult(mst->addGroup(bgm));
		ErrorCheck();
		SetResult(mst->addGroup(sfx));
		ErrorCheck();

		fCGroups.push_back(mst); // 0
		fCGroups.push_back(bgm); // 1
		fCGroups.push_back(sfx); // 2

		SetResult(UseCGroup(1)->setVolume(1.f));
		ErrorCheck();
		SetResult(UseCGroup(2)->setVolume(1.f));
		ErrorCheck();

		SetResult(pFSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fft)); // create DSP
		ErrorCheck();


		SetResult(mst->addDSP(0, fft)); // add to master channel group
		ErrorCheck();

		//SetResult(bgm->addDSP(0, fft)); // add to music channel group
		////SetResult(bgm->addDSP(FMOD_DSP_PARAMETER_DATA_TYPE_FFT, fft)); 
		//ErrorCheck();

		//SetResult(sfx->addDSP(0, fft)); // add to sfx channel group
		//ErrorCheck();

		SetResult(fft->setBypass(true)); // bypass first
		ErrorCheck();

		//fft->setParameterFloat(FMOD_DSP_FFT_WINDOWSIZE, FFT_SAMPLE_SIZE);
		//fft->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, FMOD_DSP_FFT_WINDOW_HANNING);
		fft->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, POPPLIO_AUDIO_FFT_SAMPLE_SIZE);

		SetResult(pFSystem->setDSPBufferSize(2048, 16));
	}

	FMOD::System* AudioSystem::UseSys() { return AudioSystem::pFSystem; }

	int const AudioSystem::SetAudio(char const* path, std::string const& key, FMOD::ChannelGroup* c,
		float const& beatsPM, std::pair<int, int> const& timeS)
		// return index in audio after adding
	{
		// check if path exists?

		if (audio.size() >= maxChannels) return -1; // if more than max no. of channels
		if (c != nullptr && std::find(fCGroups.begin(), fCGroups.end(), c) ==
			fCGroups.end()) // channel group not found
		{
			Logger::Error("Failed to load audio");
			return -1;
		}

		FMOD::Sound* temp{ nullptr };

		// Try to create sound
		if (pFSystem->createSound(path, fMode, nullptr, &temp) != FMOD_OK)
		{
			Logger::Error("Failed to load audio");
			return -1;
		}

		AssetStore::StoreAudioAsset(key, temp);
		audio.push_back(Audio(*this, key, c, audio.size(), false, beatsPM, timeS));

		//SetResult(audio[audio.size() - 1].UseChannel()->addDSP(0, dsp));
		//if (GetResult() != FMOD_OK) Logger::Error("AudioSystem: Error while setting up DSP for audio");

		if (Popplio::Engine::Config::verbose)
			Logger::Info("Successfully loaded audio: " + static_cast<std::string>(path));

		return static_cast<int>(audio.size()) - 1;
	}

	//todo: implement / remove
	//int const SetAudio(Audio const& rhs) // copy
	//{
	//	(void)rhs;
	//	return 0;
	//}

	int const AudioSystem::InstAudio(std::string const& id, unsigned int const& grp,
		float const& beatsPM, std::pair<int, int> const& timeS)
	{
		if (audio.size() + inst.size() >= maxChannels) return -1; // if more than max no. of channels
		if (grp >= fCGroups.size()) // channel group not found
			return -1;

        if (AssetStore::GetAudio(id) == nullptr) // error out if audio not found
			return -1;

		inst.push_back(Audio(*this, id, fCGroups[grp], inst.size(), true, beatsPM, timeS));

		return static_cast<int>(inst.size()) - 1;
	}

	void AudioSystem::SetMode(FMOD_MODE const& mode) { AudioSystem::fMode = mode; }

	void AudioSystem::SetResult(FMOD_RESULT const& result) { AudioSystem::fResult = result; }

	//void AudioSystem::SetPlaying(bool const& play) { AudioSystem::isPlaying = play; }

    std::vector<FMOD::ChannelGroup*> const& AudioSystem::GetCGroup() const { return fCGroups; }

	FMOD::ChannelGroup* const AudioSystem::GetCGroup(size_t const& i) const { return AudioSystem::fCGroups[i]; }

	FMOD::ChannelGroup* AudioSystem::UseCGroup(size_t const& i) { return AudioSystem::fCGroups[i]; }

	FMOD_RESULT const& AudioSystem::GetResult() const { return AudioSystem::fResult; }

	FMOD_MODE const& AudioSystem::GetMode() const { return AudioSystem::fMode; }

	//bool const& AudioSystem::GetPlaying() const { return AudioSystem::isPlaying; }

	Audio& AudioSystem::GetAudio(size_t const& i) 
	{ 
        if (i >= audio.size())
        {
            Logger::Warning("AudioSystem: Tried to access invalid audio!");
            return defaultAudio;
        }
		return audio[i]; 
	}
	Audio const& AudioSystem::GetAudio(size_t const& i) const 
	{ 
		if (i >= audio.size())
		{
			Logger::Warning("AudioSystem: Tried to access invalid audio!");
			return defaultAudio;
		}
		return audio[i]; 
	}

	Audio& AudioSystem::GetAudioInst(size_t const& i) 
	{ 
		if (i >= inst.size())
		{
			Logger::Warning("AudioSystem: (mutable) Tried to access invalid instance!");
			return defaultAudio;
		}
		return inst[i]; 
	}
	Audio const& AudioSystem::GetAudioInst(size_t const& i) const 
	{ 
		if (i >= inst.size())
		{
			Logger::Warning("AudioSystem: (const) Tried to access invalid instance!");
			return defaultAudio;
		}
		return inst[i]; 
	}

	Audio& AudioSystem::GetAudio(std::string const& s)
	{
		for (Audio& a : audio)
		{
			if (a.GetID() == s) return a;
		}

		Logger::Warning("AudioSystem: GetAudio returned defaultAudio!");
		return defaultAudio;
	}
	Audio const& AudioSystem::GetAudio(std::string const& s) const
	{
		for (Audio const& a : audio)
		{
			if (a.GetID() == s) return a;
		}

		Logger::Warning("AudioSystem: GetAudio returned defaultAudio!");
		return defaultAudio;
	}

	void AudioSystem::RemoveAudioInst(size_t const& i)
	{
		Logger::Debug("AudioSystem: Attempting to remove audio instance");
		if (i >= inst.size())
		{
			Logger::Warning("AudioSystem: (remove) Tried to access invalid instance!");
			return;
		}

		Logger::Debug("AudioSystem: Killing Audio Instance " + inst[i].GetID() + " : " + std::to_string(i));

		inst[i].Stop();

        inst.erase(inst.begin() + i);

        for (auto& a : inst)
        {
			for (size_t j{}; j < inst.size(); ++j)
			{
				if (inst[j] == a)
				{
					inst[j].SetIndex(j);
					break;
				}
                if (j == inst.size() - 1)
                {
                    Logger::Error("AudioSystem: Error while reindexing audio instances");
                }
			}
        }
	}

	void AudioSystem::Setup()
	{
		UpdateAudioComponents();
	}

	void AudioSystem::Init()
	{
		for (Entity& e : reg.GetAllEntitiesMutable())
		{
            if (e.HasComponent<AudioComponent>())
            {
                //entitiesWithAudio.push_back(e);
				if (!e.GetComponent<ActiveComponent>().isActive) continue;
				if (!e.GetComponent<AudioComponent>().isActive) continue;

				if (e.GetComponent<AudioComponent>().data == -1) continue;
                if (GetAudioInst(e.GetComponent<AudioComponent>().data).playOnAwake) 
					GetAudioInst(e.GetComponent<AudioComponent>().data).Play();
            }
		}

        //for (Audio& a : audio)
        //{
        //    if (a.playOnAwake) a.Play(); // play all audio tracks that are set to play on awake
        //}

		fftLen = static_cast<unsigned>(InitFFT(64, 32)); // doesnt do anything currently
	}

	void AudioSystem::Update()
	{
		for (Entity const& e : reg.GetAllEntitiesMutable())
		{
			if (!e.HasComponent<AudioComponent>()) continue;
			if (e.GetComponent<ActiveComponent>().isActive && e.GetComponent<AudioComponent>().isActive) continue;

			if (e.GetComponent<AudioComponent>().data == -1) continue;
			Audio& a = GetAudioInst(e.GetComponent<AudioComponent>().data);
            if (a.GetPlaying()) a.Pause(); // currently cant resume on enable // TODO: fix
		}

		//for (Entity const& e : entitiesWithAudio)
		//{
		//	if (!e.GetComponent<ActiveComponent>().isActive) continue;
		//	if (e.HasComponent<AudioComponent>())
		//	{
		//		if (!e.GetComponent<AudioComponent>().isActive) continue;
		//		Audio& a = e.GetComponent<AudioComponent>().data;
		//		if (a.playOnAwake) a.Play();
		//	}
		//}

		for (Audio& a : audio)
		{
			if (a.GetCurr() >= a.GetLen())
			{
				a.Stop(); // stop all finished audio tracks
				a.SetCurr(0);
				if (a.GetLoop()) a.Play();
			}
		}

        for (Audio& a : inst)
        {
			if (a.GetCurr() >= a.GetLen())
			{
				a.Stop(); // stop all finished instanced audio
				a.SetCurr(0);
				if (a.GetLoop()) a.Play();
			}
        }

		SetResult(pFSystem->update());

		fCGroups[0]->isPlaying(&(isPlaying));
	}

    void AudioSystem::End()
    {
        StopAllAudioInst();
		StopAllAudio();

        while (!inst.empty())
        {
            RemoveAudioInst(0);
        }

		//for (Audio& a : inst)
		//{
  //          RemoveAudioInst(a.GetIndex());
		//}
    }

	void AudioSystem::Terminate()
	{
		if (fft != nullptr)
		{
			fCGroups[0]->removeDSP(fft);
			fCGroups[1]->removeDSP(fft);
			fCGroups[2]->removeDSP(fft);
		}
		fft->release();
		
		pFSystem->close();
		pFSystem->release();
	}

	void AudioSystem::SubscribeToEvents(EventBus& eventBus)
	{
		static_cast<void>(eventBus);
	}

	size_t AudioSystem::InitFFT(size_t max, size_t len)
	{
		// octaves
		std::vector<unsigned> freqOct{};
		freqOct.push_back(0);

		for (size_t i{ 1 }; i < len + 1; ++i)
		{
			freqOct.push_back(static_cast<unsigned>(static_cast<float>(samples / 2) / static_cast<float>(pow(2, 12 - i))));
		}

		unsigned bWidth = samples / fftWindow;
		unsigned bandPerOct = static_cast<unsigned>(max / len);

		// calculate
		fftSamplePerBarLog.clear();

		for (size_t oct{}; oct < len; ++oct)
		{
			unsigned i1 = freqOct[oct] / bWidth; // index 1
			unsigned i2 = (freqOct[oct + 1]) / bWidth; // index 0
			unsigned iOct = i2 - i1;

			if (iOct > 0)
				if (iOct <= bandPerOct)
					for (size_t count{}; count < iOct; ++count)
						fftSamplePerBarLog.push_back(1);
			else
				for (size_t count{}; count < bandPerOct; ++count)
					fftSamplePerBarLog.push_back(iOct / bandPerOct);
		}

		return fftSamplePerBarLog.size(); // effective
	}

	void AudioSystem::StopAllAudio()
	{
		for (Audio& a : audio)
		{
			a.Stop(); // stop all audio tracks
		}
	}

	void AudioSystem::PauseAllAudio()
	{
		for (Audio& a : audio)
		{
			if (a.GetPlaying()) a.Pause(); // 
		}
	}

	void AudioSystem::PlayAllAudio()
	{
		for (Audio& a : audio)
		{
			a.Play(); // 
		}
	}

	void AudioSystem::ResumeAllAudio()
	{
		for (Audio& a : audio)
		{
			if (a.GetPaused()) a.Play();
		}
	}

	void AudioSystem::StopAllAudioInst()
	{
		for (Audio& a : inst)
		{
			a.Stop(); // stop all audio tracks
		}
	}

	void AudioSystem::PauseAllAudioInst()
	{
		for (Audio& a : inst)
		{
			if (a.GetPlaying()) a.Pause(); // 
		}
	}

	void AudioSystem::PlayAllAudioInst()
	{
		for (Audio& a : inst)
		{
			a.Play(); // 
		}
	}

	void AudioSystem::ResumeAllAudioInst()
	{
		for (Audio& a : inst)
		{
			if (a.GetPaused()) a.Play();
		}
	}

	bool AudioSystem::ErrorCheck()
	{
		if (fResult != FMOD_OK) Logger::Error("AudioSystem: Error with FMOD");

        return fResult == FMOD_OK;
	}

	void AudioSystem::UpdateAudioComponents()
	{
		for (Entity& e : reg.GetAllEntitiesMutable())
		{
            if (!e.HasComponent<AudioComponent>()) continue;
			e.GetComponent<AudioComponent>().Update(*this);
		}
	}

	// --------------------------------------------------

	//bool const AudioSystem::GetFFT(float* fft)
	//{
	//	FMOD_DSP_PARAMETER_FFT* fftDSP{ nullptr };
	//	SetResult(dsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftDSP, nullptr, nullptr, 0));

	//	if (fftDSP != nullptr)
	//	{
	//		int len = fftDSP->length / 2;
	//		int ch = fftDSP->numchannels;
	//		//dsp->getNumOutputs(&ch);
	//		//dsp->getChannelFormat(nullptr, &len, nullptr);

	//		if (len > 0)
	//		{
	//			unsigned iFFT{};

	//			for (size_t i{}; i < fftSamplePerBarLog.size(); ++i)
	//			{
	//				for (size_t j{}; j < fftSamplePerBarLog[i]; ++j)
	//				{
	//					for (size_t k{}; k < ch; ++k)
	//						fft[i] += fftDSP->spectrum[k][iFFT];

	//					++iFFT;
	//				}
	//				fft[i] /= static_cast<float>(fftSamplePerBarLog[i] * ch);
	//			}
	//		}

	//		return true;
	//	}

	//	return false;
	//}

	//FMOD_RESULT const& AudioSystem::GetFFT(Audio& a, void** fft, unsigned int& len)
	//{
	//	FMOD_RESULT res{};
	//	FMOD::Channel& c = *(a.UseChannel());

	//	//SetResult(a.UseChannel()->addDSP(0, dsp));
	//	if ((res = GetResult()) != FMOD_OK)
	//	{
	//		Logger::Warning("AudioSystem: GetFFT failed!");
	//		return res;
	//	}
	//	
	//	bool isActive;
	//	dsp->getActive(&isActive);

	//	if (!isActive) dsp->setActive(true);

	//	SetResult(dsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, 
	//		fft, &len, nullptr, 0));
	//	if ((res = GetResult()) != FMOD_OK)
	//	{
	//		Logger::Warning("AudioSystem: GetFFT failed!");
	//		return res;
	//	}

	//	if (fft != nullptr) // process FFT
	//	{
	//		fftData = (FMOD_DSP_PARAMETER_FFT*)fft;
	//		//len = static_cast<size_t>(fftData->length);

	//		if (fftData != nullptr && len != 0) return GetResult();

	//		return FMOD_ERR_BADCOMMAND;
	//	}
	//	else return FMOD_ERR_BADCOMMAND;
	//}

	// ------------------------------------------------------------------------------------- //

	// AudioComponent


}

//FMOD_RESULT const& AudioSystem::play(size_t const& index)
//{
//    if (index >= AudioSystem::audio.size()) return FMOD_ERR_INVALID_PARAM;

//    AudioSystem::fResult = AudioSystem::pFSystem->playSound(
//        std::get<FMOD::Sound*>(AudioSystem::audio[index]), nullptr, false,
//        &(std::get<FMOD::Channel*>(AudioSystem::pChannels[std::get<size_t>(AudioSystem::audio[index])])));

//    if (AudioSystem::fResult == FMOD_OK) AudioSystem::isPlaying = true;
//    else return AudioSystem::fResult;

//    std::get<bool>(AudioSystem::pChannels[std::get<size_t>(AudioSystem::audio[index])]) = true;

//    return AudioSystem::fResult;
//}
