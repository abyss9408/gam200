/******************************************************************************/
/*!
\file   AudioSystem.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 98% Val - 2%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
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

#pragma once

#include <fmod.hpp>
#include <fmod_common.h>

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"

#include <vector>
#include <string>
#include <tuple>

#define POPPLIO_AUDIO_MAX_CHANNELS 100

#define POPPLIO_AUDIO_BEAT_THRESHOLD 5 // in ms

#define POPPLIO_AUDIO_FFT_BIN_COUNT 32

#define POPPLIO_AUDIO_FFT_SAMPLE_SIZE 512

#define POPPLIO_AUDIO_FFT_MIN_FREQ 20.f

namespace Popplio
{
	// FMOD uses FMOD_TIMEUNIT (ms)
	// Interface uses unsigned int / float (ms)

	// todo: temp
	//constexpr const char* SOUND_TRACK { "track" };
	//constexpr const char* SOUND_CLAP { "clap_sfx" };

	constexpr const char* AUDIO_DEFAULT { "" };

	class AudioSystem;

	/*
    *   Audio object that stores FMOD::Sound and FMOD::Channel
    *	to be played by AudioSystem.
	*/
	class Audio 
	{ // Don't interact with directly outside of AudioSystem! todo: enforce
	private:

		static unsigned int const beatThreshold{ POPPLIO_AUDIO_BEAT_THRESHOLD }; // in ms

		AudioSystem& rSystem;

		//FMOD::Sound* pAudio;
		std::string audioID; // ID of audio in asset store

		FMOD::Channel* pChannel; // todo: change to vector to play to multiple components

		FMOD::ChannelGroup* group;
		int groupIndex;

		size_t index; // index of audio in audio system vector

		bool isInstance;

		// Audio info
		float volume;					// serialize
        bool mute;                      // serialize
		bool loop;						// serialize

		float samplerate;

		// Additional info

		// Beats per Minute
		float bpm;						// serialize
		float beatInterval;

		// Time signature
        // first (top) = beats, second (bot) = sub notes
		std::pair<int, int> timeSig;	// serialize

		// Runtime info
		bool isInThreshold;

		//bool isPlaying;
		//bool isPaused;

		// FFT
		std::vector<float> logBins{};

	public:

		// Additional info

        bool playOnAwake;				// serialize	

		// Constant

		static FMOD_TIMEUNIT const defaultUnit{ FMOD_TIMEUNIT_MS };


		//Audio(AudioSystem& sys, FMOD::Sound* audio, FMOD::ChannelGroup* grp, size_t const& i,
		//	float const& beatsPM, std::pair<int, int> const& timeS);

		/*
		*   Gets play status of Audio instance
		* 
        *	@return play status of audio
		*/
		bool GetPlaying();
		/*
		*   Gets pause status of Audio instance
		* 
		*	@return pause status of audio
		*/
		bool GetPaused();
		/*
		*   Gets stop status of Audio instance
		*
		*	@return stop status of audio
		*/
		bool GetStopped();

		// disable
		Audio() = delete;

		// arg constructor // tested
		/*
		*   Audio argument constructor
        *	Properly Initializes Audio object
        *	To only be used by AudioSystem
        * 
        *	@param sys			| reference to AudioSystem
        *	@param audioID		| string id of audio
        *	@param grp			| FMOD::ChannelGroup to play audio
        *	@param i			| index of audio in AudioSystem
        *	@param instanced	| is audio instanced
        *	@param beatsPM		| BPM of audio (if applicable)
        *	@param timeS		| time signature of audio (if applicable)
		*/
		Audio(AudioSystem& sys, std::string audioID, FMOD::ChannelGroup* grp, size_t const& i,
			bool const& instanced, float const& beatsPM = 0.f, std::pair<int, int> const& timeS = { 0, 0 });

		// copy constructor // unsure
		/*
		*   Audio copy constructor
		*	Properly Initializes Audio object
		*	To only be used by AudioSystem
		*
		*	@param rhs | Audio to copy from
		*/
		Audio(Audio const& rhs);

		// assignment operator default // untested
		Audio& operator=(Audio const& rhs); //= default;

        bool operator==(Audio& rhs);

		// destructor // untested
		~Audio() = default;

		//------------------

		//FMOD::Sound* useAudio();

		// audio string id

		/*
        *   Accesses FMOD::Sound* of Audio instance
        *	Audio data of the object
		*
        *	@return Sound pointer
		*/
		FMOD::Sound* UseSound();
		/*
		*   Accesses FMOD::Channel* of Audio instance
		*	Channel data of the object
		*
		*	@return Channel pointer
		*/
		FMOD::Channel* UseChannel();

		/*
		*   Get FMOD::ChannelGroup* of Audio instance
        *	Pointer to which channel group the audio is in
		*
		*	@return ChannelGroup pointer
		*/
		FMOD::ChannelGroup* GetGroup() const;
		/*
		*   Set FMOD::ChannelGroup* of Audio instance
		*	Set which channel group the audio is in (e.g. BGM, SFX, MASTER)
		*
		*	@param pointer to FMOD::ChannelGroup to set this instance to
		*/
		void SetGroup(FMOD::ChannelGroup* grp);

		/*
		*   Get index of FMOD::ChannelGroup of Audio instance
		*	0 = Master, 1 = BGM, 2 = SFX
		* 
		*	@return ChannelGroup index
		*/
		int GetGroupByIndex() const;
		/*
		*   Set index of FMOD::ChannelGroup of Audio instance
		*	Set which channel group the audio is in (e.g. BGM, SFX, MASTER)
		*	0 = Master, 1 = BGM, 2 = SFX
		*
		*	@param pointer to FMOD::ChannelGroup to set this instance to
		*/
		void SetGroupByIndex(int const& grp);

		/*
        *   Get status of FMOD::Channel* play status
		*	Different from GetPlay()
		*
        *	@return play status of FMOD::Channel*
		*/
		//bool const GetPlaying();
		/*
        *   Get index of Audio instance in AudioSystem
		*
		*	@return index
		*/
		size_t const& GetIndex() const;

        void SetIndex(size_t const& i);

		/*
		*   Get volume of Audio instance (not current volume of FMOD::Channel*)
		*
		*	@return volume
		*/
        float const& GetVolume() const;
		/*
		*   Set volume of Audio instance (not current volume of FMOD::Channel*)
		*
        *	@param vol | volume to set
		* 
		*	@return volume
		*/
		float const& SetVolume(float const& vol);

		bool const& GetMute() const;
		bool const& SetMute(bool const& m);

		/*
		*   Get loop of Audio instance
		*
		*	@return loop
		*/
		bool const& GetLoop();
		/*
		*   Set loop of Audio instance
		*
		*	@param doLoop | bool to set
		*
		*	@return loop
		*/
		bool const& SetLoop(bool const& doLoop);
		/*
		*   Get length of Audio track
		*
        *	@return length of track
		*/
		unsigned int GetLen() const;

		/*
		*   Get BPM of Audio track
		*
		*	@return BPM of track
		*/
        float const& GetBPM() const;
		/*
		*   Set BPM of Audio track
		* 
        *	@param b | BPM to set
		*
		*	@return BPM of track
		*/
        float const& SetBPM(float const& b);
		/*
		*   Get Beat Interval of Audio track
		*
		*	@return Beat Interval of track
		*/
        float const& GetBeatInterval() const;
		/*
		*   Set Time Signature of Audio track
		*
		*	@param ts | Time Signature to set
		*
		*	@return Time Signature of track
		*/
        std::pair<int, int> const& SetTimeSig(std::pair<int, int> const& ts);
		/*
		*   Get Time Signature of Audio track
		*
		*	@return Time Signature of track
		*/
        std::pair<int, int> const& GetTimeSig() const;

		/*
		*   Get string identifier of object
		*
		*	@return ID
		*/
		std::string const& GetID() const;

		/*
		*   Set string identifier of object
		*	Might lead to errors
		*
		*	@return ID
		*/
		std::string const& SetID(std::string const& s);

		/*
		*   Accesses AudioSystem from this instance
		*
		*	@return reference to AudioSystem
		*/
		AudioSystem& UseSys();
		/*
		*   Get AudioSystem from this instance
		*
		*	@return read-only reference to AudioSystem
		*/
		AudioSystem const& GetSys() const;

        float const& GetSampleRate();

		//------------------

		/*
		*   Get current playhead location (USE SAME TIME UNIT AS len)
		*
		*	@return read-only current playhead location
		*/
		unsigned int const GetCurr(); 
		/*
		*   Set current playhead location (USE SAME TIME UNIT AS len)
		*
		*	@param current playhead location to set
		*/
		void SetCurr(unsigned int const& pos);

		/*
        *   Check if next beat has reached within threshold
		*
		*	@return bool if on beat
		*/
		bool const IsOnBPM();

		//------------------

		/*
		*   Play the audio this instance points to
		*
		*	@return result status of call
		*/
		FMOD_RESULT const& Play();
		/*
		*   Pause the audio this instance points to
		*
		*	@return result status of call
		*/
		FMOD_RESULT const& Pause();
		/*
		*   Stop the audio this instance points to
		*
		*	@return result status of call
		*/
		FMOD_RESULT const& Stop();

		/*
        *   Gets how off beat the current time is
		*
        *	@return beat accuracy in ms
		*/
		int const OffBeat(); 

		/*
		*   Get  Fast Fourier Transform (FFT) array data
		*
		*	@param fft | FFT data
		* 
        *	@param magMod | magnitude of FFT data
		* 
		*	@return if call is successful
		*/
		bool const GetSpectrum(float** fft, float magMod);

		/*
		*   Get  Fast Fourier Transform (FFT) array data
        *	Logarithmic representation
		*
		*	@param fft | FFT data
		*
		*	@param magMod | magnitude of FFT data
		*
		*	@return if call is successful
		*/
		bool const GetSpectrumLog(float** fft, float magMod);

		/*
        *   Kills instance of audio
		*/
		void Kill();

        // TODO: MIDI
	};

	//---------------------------------------------

	/*
	*   AudioSystem to encapsulate all FMOD and related audio functionality
	*/
	class AudioSystem : public System
	{
	private:
		// System
		Registry& reg;

        // FMOD data
		unsigned int version;

		FMOD_RESULT fResult;
		FMOD_MODE fMode;

		std::vector<FMOD::ChannelGroup*> fCGroups; // 0 = Master, 1 = BGM, 2 = SFX

		FMOD::System* pFSystem;

		size_t maxChannels;

		bool isPlaying;

		// audio data
		std::vector<Audio> audio; 

        std::vector<Audio> inst; // instantiated audio

		// runtime
		//std::vector<Entity&> entitiesWithAudio;

		//static size_t const fftSampleSize{ 256 };

		//float fft[fftSampleSize]{};

	public:

		static FMOD_TIMEUNIT const defaultUnit{ FMOD_TIMEUNIT_MS };

		// FFT
		FMOD::DSP* fft{ nullptr };
		FMOD_DSP_PARAMETER_FFT* fftData{ nullptr };
		float spectrum[POPPLIO_AUDIO_FFT_BIN_COUNT]{};

		static unsigned const samples{ 44100 };
		static unsigned const fftWindow{ 441 };
		std::vector<unsigned> fftSamplePerBarLog{};

		unsigned fftLen{};

		Audio defaultAudio;

		//------------------

		/*
		*   AudioSystem argument constructor
		*/
		AudioSystem(Registry& r);
		// dunno if should do anything to FMOD::System* when exiting
		//~AudioSystem();

		//------------------

		// Getters / Setters

		/*
        *   Access FMOD::System* of AudioSystem
		*
		*	@return system pointer
		*/
		FMOD::System* UseSys();

		/*
        *   Adds an Audio object to the AudioSystem
        * 
		*	@param path		| path location of file
		*	@param key		| string id of audio
		*	@param c		| FMOD::ChannelGroup to play audio
		*	@param beatsPM	| BPM of audio (if applicable)
		*	@param timeS	| time signature of audio (if applicable)
		* 
		*	@return index of new Audio
		*/
		int const SetAudio(char const* path, std::string const& key, FMOD::ChannelGroup* c,
		float const& beatsPM = 0.f, std::pair<int, int> const& timeS = { 0, 0 });
		/*
		*   Adds an Audio object to the AudioSystem from a copy
		*
        *	@param rhs | Audio to copy from
		*
		*	@return index of new Audio
		*/
		//int const SetAudio(Audio const& rhs); 

		/*
		*   Instantiates an Audio instance in the AudioSystem
		*
		*	@param id		| string id of audio
		*	@param grp		| FMOD::ChannelGroup to play audio
		*	@param beatsPM	| BPM of audio (if applicable)
		*	@param timeS	| time signature of audio (if applicable)
		*
		*	@return index of new Audio
		*/
		int const InstAudio(std::string const& id, unsigned int const& grp = 0,
			float const& beatsPM = 0.f, std::pair<int, int> const& timeS = { 0, 0 });

		/*
		*   Set fmod_mode of AudioSystem
		*
		*	@param mode | mode to set
		*/
		void SetMode(FMOD_MODE const& mode); 
		/*
        *   Set fResult of AudioSystem
        *	Called internally in class after FMOD calls
		*
		*	@param result | result to set
		*/
		void SetResult(FMOD_RESULT const& result); 
		/*
		*   Set play status of AudioSystem
		*
		*	@param play | status to set
		*/
		//void SetPlaying(bool const& play); 

		/*
		*   Get ChannelGroup container
		*
		*	@return read-only ChannelGroup pointer container
		*/
		std::vector<FMOD::ChannelGroup*> const& GetCGroup() const;
		/*
        *   Get ChannelGroup at index given
		*
        *	@param i | index of ChannelGroup
		*
		*	@return read-only ChannelGroup pointer
		*/
		FMOD::ChannelGroup* const GetCGroup(size_t const& i) const;
		/*
		*   Use ChannelGroup at index given
		*
		*	@param i | index of ChannelGroup
		*
		*	@return ChannelGroup pointer
		*/
		FMOD::ChannelGroup* UseCGroup(size_t const& i);

		/*
        *   Get fmod_mode of AudioSystem
		*
		*	@return fmod_mode
		*/
		FMOD_MODE const& GetMode() const;
		/*
		*   Get fmod_result stored in AudioSystem
		*
		*	@return fmod_result
		*/
		FMOD_RESULT const& GetResult() const;
		/*
		*   Get if any Audio is playing in AudioSystem
		*
		*	@return status of AudioSystem
		*/
		//bool const& GetPlaying() const;

		/*
		*   Access Audio object at index
		*
        *	@param i | index of Audio
		* 
		*	@return Audio reference
		*/
		Audio& GetAudio(size_t const& i);
		/*
		*   Get Audio object at index
		*
		*	@param i | index of Audio
		*
		*	@return read-only Audio reference
		*/
		Audio const& GetAudio(size_t const& i) const;

		/*
		*   Access Audio instance at index
		*
		*	@param i | index of Audio
		*
		*	@return Audio reference
		*/
		Audio& GetAudioInst(size_t const& i);
		/*
		*   Get Audio instance at index
		*
		*	@param i | index of Audio
		*
		*	@return read-only Audio reference
		*/
		Audio const& GetAudioInst(size_t const& i) const;

		/*
		*   Access Audio object with its string ID
		*
		*	@param s | ID of Audio
		*
		*	@return Audio reference
		*/
		Audio& GetAudio(std::string const& s);
		/*
		*   Get Audio object with its string ID
		*
		*	@param s | ID of Audio
		*
		*	@return read-only Audio reference
		*/
		Audio const& GetAudio(std::string const& s) const;

		/*
		*   Remove Audio instance with its index
		*
		*	@param i | index of instance
		*
		*	@return void
		*/
		void RemoveAudioInst(size_t const& i);

		//FMOD_RESULT const& play(size_t const& index);

		//------------------

		// State Functions

		/*
        *   Sets up AudioSystem for runtime
		*
		*	@return void
		*/
		void Setup();

		/*
        *   Initializes the AudioSystem
        *	Sets up FMOD system
		*/
		void Init();

		/*
        *   Update status of AudioSystem
		*/
		void Update();

		/*
		*   End status of AudioSystem
		*/
		void End();

		/*
		*   Cleanup and shutdown AudioSystem
		*/
		void Terminate();

		//------------------

		// Helper Functions

		/*
        *   Subscribe to eventbus to be called by events
		*
        *	@param eventBus | Engine's event bus
		*/
		void SubscribeToEvents(EventBus& eventBus);

		/*
        *   Initialize the Fast Fourier Transform (FFT) algorithm for AudioSystem
		*	Currently not working // TODO FIX
		* 
		*	@param max | maximum samples
		*	@param len | count of discrete bars
		*/
		size_t InitFFT(size_t max, size_t len);

		/*
		*   Get  Fast Fourier Transform (FFT) array data
		*	Currently not working // TODO FIX
		*
		*	@param fft | FFT data
		*/
		//bool const GetFFT(float* fft);

		/*
        *   Stops all Audio in AudioSystem
		*/
		void StopAllAudio();

		/*
		*   Pauses all Audio in AudioSystem
		*/
		void PauseAllAudio();

		/*
		*   Plays all paused Audio in AudioSystem
		*/
		void PlayAllAudio();

		/*
		*   Resumes all paused Audio in AudioSystem
		*/
		void ResumeAllAudio();

		/*
        *   Accesses Audio container in AudioSystem
		*
		*	@return reference to Audio vector
		*/
		std::vector<Audio>& GetAllAudio() { return audio; }

		/*
		*   Stops all Audio Instances in AudioSystem
		*/
		void StopAllAudioInst();

		/*
		*   Pauses all Audio Instances in AudioSystem
		*/
		void PauseAllAudioInst();

		/*
		*   Plays all paused Audio Instances in AudioSystem
		*/
		void PlayAllAudioInst();

		/*
		*   Resumes all paused Audio Instances in AudioSystem
		*/
		void ResumeAllAudioInst();

		/*
		*   Accesses Audio Instance container in AudioSystem
		*
		*	@return reference to Audio vector
		*/
		std::vector<Audio>& GetAllAudioInst() { return inst; }

		// FMOD_RESULT const& GetFFT(Audio& a, void** fft, unsigned int& len);

		/*
		*   Checks for FMOD errors
		*
        *	@return true if no errors
		*/
		bool ErrorCheck();

		/*
        *   Update Audio Components in ECS
		*/
		void UpdateAudioComponents();
	};
}
