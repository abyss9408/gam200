using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    [Header("Fields")]
    [SerializeField]
    AudioManagerScript audioManager;
    [SerializeField]
    Track track;

    [Header("Settings")]
    [SerializeField]
    public float musicDelay = 3f;

    [SerializeField]
    public float startTime = 0f; // for testing, otherwise set to 0

    [SerializeField]
    public float resultDelay = 3f;

    float timer = 0f;

    bool ended = false;

    void Awake()
    {
        musicDelay = audioManager.GetBeatInterval(audioManager.music, 0) * 
            Mathf.Floor(musicDelay / audioManager.GetBeatInterval(audioManager.music, 0));
        timer = 0f;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Escape)) Application.Quit();
        if (Input.GetKey(KeyCode.Tab)) // reset scene
        {
            Scene scene = SceneManager.GetActiveScene();
            SceneManager.LoadScene(scene.name);
        }

        if (ended)
        {
            timer += Time.deltaTime;
            if (timer >= resultDelay) SceneManager.LoadScene("Results");
            return;
        }

        if (audioManager.music[0].source.time >= audioManager.music[0].clip.length)
        {
            // todo: end current level
            ended = true;
            timer = 0f;
            return;
        }

        if (audioManager.music[0].source.isPlaying && audioManager.music[1].source.isPlaying) return;

        if (timer >= musicDelay) timer += Time.deltaTime;

        if(timer >= musicDelay + track.customAudioOffset) audioManager.PlayMusic(); // actual audio

        // delay music
        if (audioManager.music[0].source.isPlaying) return;

        timer += Time.deltaTime;

        if (timer >= musicDelay)
        {
            audioManager.Play(audioManager.music, 0); // base anchor
            audioManager.music[0].source.time = startTime;
        }
    }

    void FixedUpdate()
    {

    }
}
