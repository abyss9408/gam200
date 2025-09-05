using System.Linq;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Track : MonoBehaviour
{
    #region variables

    [Header("Fields")]
    [SerializeField]
    public AudioManagerScript audioManager;
    [SerializeField]
    GameManager gameManager;
    [SerializeField]
    public JudgementInput judgement;
    [SerializeField]
    public Score score;
    [SerializeField]
    Transform notes;

    [SerializeField]
    GameObject lane1;
    [SerializeField]
    GameObject lane2;
    [SerializeField]
    GameObject lane3;

    [Header("Notes")]
    public List<NoteProperties> notesLane1;
    public List<NoteProperties> notesLane2;
    public List<NoteProperties> notesLane3;

    [Header("Settings")]
    [SerializeField]
    public float customSpeed = 1f;
    [SerializeField]
    public float customOffset = 0f; 
    [SerializeField]
    public float customAudioOffset = 0f; 

    [SerializeField]
    float speedConst = 1f;

    // vars
    [HideInInspector]
    public float speed; // speed
    [HideInInspector]
    public Vector3 noteStart; // note parent start pos
    [HideInInspector]
    public float length; // length

    // UI
    [Header("UI")]
    [SerializeField]
    public GameObject noteFeedback;
    [SerializeField]
    public Canvas canvas;
    [SerializeField]
    public Fever fever;

    [Header("Audio")]
    [SerializeField]
    public float muteTime = 3f;

    #endregion

    //todo: optimize track (conveyor belt system with set number of objects)

    void Awake()
    {
        // initialize

        speed = customSpeed * speedConst;

        noteStart = new Vector3(notes.position.x + customOffset * speed, notes.position.y, notes.position.z);

        length = speed * audioManager.music[0].clip.length;

        // lanes

        notesLane1 = new List<NoteProperties>();
        notesLane2 = new List<NoteProperties>();
        notesLane3 = new List<NoteProperties>();

        // add notes to lane arrays

        for (int i = 0; i < lane1.transform.childCount; i++)
        {
            notesLane1.Add(lane1.transform.GetChild(i).GetComponent<NoteProperties>());
        }
        for (int i = 0; i < lane2.transform.childCount; i++)
        {
            notesLane2.Add(lane2.transform.GetChild(i).GetComponent<NoteProperties>());
        }
        for (int i = 0; i < lane3.transform.childCount; i++)
        {
            notesLane3.Add(lane3.transform.GetChild(i).GetComponent<NoteProperties>());
        }

        GameObject start = Instantiate(new GameObject(), noteStart, Quaternion.identity, notes.transform);
        start.name = "start";
        GameObject end = Instantiate(new GameObject(), new Vector3(noteStart.x + length, noteStart.y, noteStart.z), 
            Quaternion.identity, notes.transform);
        end.name = "end";
    }

    void Start()
    {
        // order by position

        notesLane1 = notesLane1.OrderBy(o=>o.trackPos).ToList();
        notesLane2 = notesLane2.OrderBy(o=>o.trackPos).ToList();
        notesLane3 = notesLane3.OrderBy(o=>o.trackPos).ToList();

        notes.position = 
            new Vector3(notes.position.x + (((-gameManager.startTime + gameManager.musicDelay + customOffset) / 
            audioManager.music[0].clip.length) * length), notes.position.y, notes.position.z);

        foreach (NoteProperties note in notesLane1)
        {
            if (note.trackPos > 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(false);
        }
        foreach (NoteProperties note in notesLane2)
        {
            if (note.trackPos > 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(false);
        }
        foreach (NoteProperties note in notesLane3)
        {
            if (note.trackPos > 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(false);
        }
    }

    void FixedUpdate()
    {
        if (!audioManager.music[0].source.isPlaying) return; // if not playing, do nothing

        MissNotes();
    }

    void Update()
    {
        foreach (NoteProperties note in notesLane1)
        {
            if (note.trackPos - audioManager.music[0].source.time < 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(true);
        }
        foreach (NoteProperties note in notesLane2)
        {
            if (note.trackPos - audioManager.music[0].source.time < 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(true);
        }
        foreach (NoteProperties note in notesLane3)
        {
            if (note.trackPos - audioManager.music[0].source.time < 5f) // hardcoded for now todo: softcode
                note.gameObject.SetActive(true);
        }

        MoveTrack();
    }

    void MoveTrack()
    {
        // translate notes
        if (!audioManager.music[0].source.isPlaying) // if not playing
        {
            notes.Translate(Vector3.left * speed * Time.deltaTime);
        }
        else
        {
            notes.position = new Vector3(
                noteStart.x - ((audioManager.music[0].source.time / audioManager.music[0].clip.length) 
                    * length) , noteStart.y, noteStart.z);
        }
    }

    void MissNotes()
    {
        // miss notes if past threshold 
        // todo: use better system of queues to enqueue notes being pressed instead
        if (notesLane1.Count > 0 && notesLane1[0].trackPos + judgement.mehThreshold 
            < audioManager.music[0].source.time)
        {
            if (!(notesLane1[0].type == 1 && judgement.onHoldNote[0])) // don't miss hold notes being held
            {
                if (notesLane1[0].trackPos <= gameManager.startTime) notesLane1[0].judgement = 3;

                GameObject obj = notesLane1[0].gameObject;

                notesLane1.RemoveAt(0);
                Destroy(obj);
                score.UpdateScore(0);
            }
        }
        if (notesLane2.Count > 0 && notesLane2[0].trackPos + judgement.mehThreshold 
            < audioManager.music[0].source.time)
        {
            if (!(notesLane2[0].type == 1 && judgement.onHoldNote[1])) // don't miss hold notes being held
            {
                if (notesLane2[0].trackPos <= gameManager.startTime) notesLane2[0].judgement = 3;

                GameObject obj = notesLane2[0].gameObject;

                notesLane2.RemoveAt(0);
                Destroy(obj);
                score.UpdateScore(0);
            }
        }
        if (notesLane3.Count > 0 && notesLane3[0].trackPos + judgement.mehThreshold 
            < audioManager.music[0].source.time)
        {
            if (!(notesLane3[0].type == 1 && judgement.onHoldNote[2])) // don't miss hold notes being held
            {
                if (notesLane3[0].trackPos <= gameManager.startTime) notesLane3[0].judgement = 3;

                GameObject obj = notesLane3[0].gameObject;

                notesLane3.RemoveAt(0);
                Destroy(obj);
                score.UpdateScore(0);
            }
        }
    }
}
