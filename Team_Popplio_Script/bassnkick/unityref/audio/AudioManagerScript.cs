using UnityEngine.Audio;
using System;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AudioManagerScript : MonoBehaviour
{
    [Header("Fields")]
    [SerializeField]
    GameManager gameManager;

    [Header("Audio")]
    public Sound[] music;
    public Sound[] sound;

    public static AudioManagerScript instance;

    [Header("Music Info")]
    public float bpm = 170f;
    public int timeSigTop = 4;
    public int timeSigBottom = 4;

    [HideInInspector]
    public float[] mLayerTimer;

    void Awake()
    {   
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(gameObject);
            return;
        }
        
            DontDestroyOnLoad(gameObject);
        
       // SceneManager.sceneLoaded += OnSceneLoaded;

        foreach (Sound s in sound) // initialize sfx
        {
            s.source = gameObject.AddComponent<AudioSource>();
            s.source.clip = s.clip;

            s.source.volume = s.volume;
            s.source.pitch = s.pitch;
            s.source.loop = s.loop;
        }
        foreach (Sound s in music) // initialize music
        {
            s.source = gameObject.AddComponent<AudioSource>();
            s.source.clip = s.clip;

            s.source.volume = s.volume;
            s.source.pitch = s.pitch;
            s.source.loop = s.loop;
        }

        mLayerTimer = new float[music.Length - 1];
    }


    private void Update()
    {
        if (!music[1].source.isPlaying) // metronome
        {
            if (isOnBPMActualTime(music, 1)) Play(sound, 0);
            return;
        }
        // if (isOnBPM(0)) Play(1);

        for (int i = 0; i < music.Length - 1; ++i)
        {
            if (mLayerTimer[i] > 0f)
            {
                if (!music[i + 1].source.mute) music[i + 1].source.mute = true;
                mLayerTimer[i] -= Time.deltaTime;
            }
            if (mLayerTimer[i] < 0f)
            {
                music[i + 1].source.mute = false;
                mLayerTimer[i] = 0f;
            }
        }
    }

    public void PlayMusic() // public interface
    {
        Play(music, 1); music[1].source.time = gameManager.startTime;
        Play(music, 2); music[2].source.time = gameManager.startTime;
        Play(music, 3); music[3].source.time = gameManager.startTime;
        Play(music, 4); music[4].source.time = gameManager.startTime;
        Play(music, 5); music[5].source.time = gameManager.startTime;
    }

    // thresholds
    bool isInThreshold = false;
    int beatThreshold = 50;

    float beatInterval = 0f;
    float currTime = 0f;

    public float GetBeatInterval(Sound[] grp, int index)
    {
        beatInterval = grp[index].source.clip.length / (bpm * (grp[index].source.clip.length / 60f));
        return beatInterval;
    }

    bool isOnBPM(Sound[] grp, int index) // return if on bpm for audio provided (based on track time)
    {
        if (bpm <= 0) return false;

        GetBeatInterval(grp, index);
        currTime = grp[index].source.time;

        // Expensive ?
        if (!isInThreshold && beatThreshold > (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
        {
            isInThreshold = true;
            return true;
        }

        if (isInThreshold && beatThreshold <= (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
            isInThreshold = false;

        return false;
    }

    bool isOnBPMActualTime(Sound[] grp, int index) // return if on bpm for audio provided (based on program time)
    {
        if (bpm <= 0) return false;
        if (Time.time <= 0f) return false;

        GetBeatInterval(grp, index);
        currTime = Time.time;

        // Expensive ?
        if (!isInThreshold && beatThreshold > (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
        {
            isInThreshold = true;
            return true;
        }

        if (isInThreshold && beatThreshold <= (int)(currTime * 1000f) % (int)(beatInterval * 1000f))
            isInThreshold = false;

        return false;
    }

    public void Play(Sound[] grp, int i) // play audio
    {
        if(i >= grp.Length || i < 0)
        {
            Debug.LogWarning("Sound" + i + "not found");
            return;
        }
        grp[i].source.Play();
    }

    public void Play(Sound[] grp, string name) // play audio
    {
        Sound s = Array.Find(grp, sound => sound.name == name);
        if(s == null)
        {
            Debug.LogWarning("Sound" + name + "not found");
            return;
        }
        s.source.Play();
    }
}
