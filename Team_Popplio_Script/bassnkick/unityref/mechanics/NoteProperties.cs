using System.Collections.Generic;
using UnityEngine;

public class NoteProperties : MonoBehaviour
{
    [Header("Fields")]
    [SerializeField]
    Track track;
    [SerializeField]
    GameObject feedback;
    [SerializeField]
    Transform holdEnd; // end pos if this note is hold
    [SerializeField]
    Transform sprite;
    [SerializeField]
    GameObject holdSprite;
    [SerializeField]
    Transform holdSpriteEnd;

    [SerializeField]
    SpriteRenderer instrument;

    [SerializeField]
    List<Sprite> instrumentSprites = new List<Sprite>();

    [Header("Runtime Properties")]
    public float trackPos = 0f; // track pos in seconds
    public float trackLength = 0f; // hold length in seconds

    public int judgement; // 0 = miss, 1 = meh, 2 = good, 3 = perfect

    [Header("Properties")]
    public int barStart = 0; // temporary for convenience (for repeated sections)
    public int bar = 0;
    public int beat = 0;
    public int beatFractionTop = 0;
    public int beatFractionBot = 0;
    public int beatLength = 0;
    public int beatLengthFractionTop = 0;
    public int beatLengthFractionBot = 0;

    public int type = 0; // 0 = tap, 1 = hold, 2 = dodge

    public bool fever = false;

    public int musicLayer = 0; // which layer / instrument this note is linked to ( 0 is none )

    // private

    bool appQuit = false;

    void Awake()
    {
        // initialize
        bar += barStart; // temporary for convenience (for repeated sections)

        trackPos = 60f / track.audioManager.bpm * (float)(bar * track.audioManager.timeSigTop);
        trackPos += 60f / track.audioManager.bpm * (float)beat;
        if (beatFractionBot > 0 && beatFractionTop > 0) 
            trackPos += 60f / track.audioManager.bpm * ((float)beatFractionTop / (float)beatFractionBot);

        transform.localPosition = 
            new Vector3(trackPos * track.customSpeed, transform.localPosition.y, transform.localPosition.z); 
            // update pos to new length if custom speed != 1f

        judgement = 0;

        feedback.GetComponent<NoteFeedback>().canvas = track.canvas;

        if (type == 1) // set up hold ui
        {
            trackLength = 60f / track.audioManager.bpm * (float)beatLength;
            if (beatLengthFractionBot > 0 && beatLengthFractionTop > 0) 
                trackLength += 60f / track.audioManager.bpm * ((float)beatLengthFractionTop / (float)beatLengthFractionBot);

            holdEnd.gameObject.SetActive(true);
            holdEnd.localPosition = new Vector3(trackLength * track.customSpeed, 0f, 0f);

            holdSprite.SetActive(true);
            holdSprite.transform.localScale = new Vector3(
                holdSprite.transform.localScale.x * (trackLength * track.customSpeed) + 1f, 
                holdSprite.transform.localScale.y, holdSprite.transform.localScale.z);

            holdSprite.transform.localPosition = new Vector3((trackLength * track.customSpeed) / 2f,
                transform.localPosition.y, transform.localPosition.z);

            holdSpriteEnd.gameObject.SetActive(true);
            holdSpriteEnd.localPosition = holdEnd.localPosition;
        }

        // if (musicLayer > 0)
        // {
        //     mLayer = track.audioManager.music[musicLayer];

        //     // sprite.GetComponent<SpriteRenderer>().color = holdSpriteEnd.GetComponent<SpriteRenderer>().color = 
        //     //     Color.HSVToRGB((float)musicLayer / (float)track.audioManager.music.Length, .2f, 1f);
        // }
    }

    void Start()
    {
        switch (musicLayer)
        {
            case 1:
            instrument.sprite = instrumentSprites[0];
            break;
            case 2:
            instrument.sprite = instrumentSprites[1];
            break;
            case 3:
            instrument.sprite = instrumentSprites[3];
            break;
            case 4:
            instrument.sprite = instrumentSprites[2];
            break;
            case 5:
            instrument.sprite = instrumentSprites[3];
            break;
        }
    }

    void OnApplicationQuit()
    {
        appQuit = true;
    }

    void OnDestroy() // feedback here
    {
        track.audioManager.sound[musicLayer - 1].source.Play();

        if (appQuit || track.audioManager == null) return; // if app quit, do nothing

        //todo: score
        
        //feedback
        switch(judgement)
        {
            case 0:
            feedback.GetComponent<NoteFeedback>().judge = "miss";
            track.score.UpdateScore(0);
            break;
            case 1:
            feedback.GetComponent<NoteFeedback>().judge = "meh";
            track.score.UpdateScore(1);
            break;
            case 2:
            feedback.GetComponent<NoteFeedback>().judge = "great";
            track.score.UpdateScore(2);
            break;
            case 3:
            feedback.GetComponent<NoteFeedback>().judge = "perfect";
            track.score.UpdateScore(3);
            break;
        }
        feedback.transform.SetParent(null, true); // prevent feedback from getting destroyed with note

        if (type == 1) // hold
        {
            feedback.transform.position =
                new Vector3(track.judgement.transform.position.x, feedback.transform.position.y, feedback.transform.position.z);
        }
        
        feedback.GetComponent<NoteFeedback>().enabled = true;

        // fever feedback
        if (fever && feedback.GetComponent<NoteFeedback>().judge != "miss") track.fever.feedback();

        // mute when miss, unmute when hit
        if (musicLayer != 0)
        {
            if (judgement < 2) track.audioManager.mLayerTimer[musicLayer - 1] = track.muteTime;
            else track.audioManager.mLayerTimer[musicLayer - 1] = -1f;
        }
    }
}
