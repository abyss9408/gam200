using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JudgementInput : MonoBehaviour
{
    #region variables

    [Header("Fields")]
    [SerializeField]
    Track trackObj;
    [SerializeField]
    PlayerController player;

    [SerializeField]
    GameObject pSys1;
    [SerializeField]
    GameObject pSys2;
    [SerializeField]
    GameObject pSys3;

    // lane 1 -> top
    // lane 2 -> mid
    // lane 3 -> bot

    // input states

    public Dictionary<int, bool> lanePress;
    public Dictionary<int, bool> laneHold;

    // keys
    [Header("Settings")]
    [SerializeField]
    KeyCode lane1Key = KeyCode.LeftShift;
    [SerializeField]
    KeyCode lane2Key = KeyCode.Space;
    [SerializeField]
    KeyCode lane3Key = KeyCode.RightShift;

    [Header("Judgement")]
    [SerializeField]
    public float perfectThreshold = .08f;
    [SerializeField]
    public float greatThreshold = .18f;
    [SerializeField]
    public float mehThreshold = .3f;

    [Header("Update Buffer")]
    [SerializeField]
    float pressThreshold = .1f;

    [Header("States")]

    float currTime; // current time

    // timer for press state
    float keyPressTimer1, keyPressTimer2, keyPressTimer3;

    bool isInThreshold = false;

    // hold note check
    public bool[] onHoldNote;

    #endregion

    // Start is called before the first frame update
    void Start()
    {
        // initialize
        lanePress = new Dictionary<int, bool>();
        laneHold = new Dictionary<int, bool>();

        lanePress.Add(1, false);
        lanePress.Add(2, false);
        lanePress.Add(3, false);

        laneHold.Add(1, false);
        laneHold.Add(2, false);
        laneHold.Add(3, false);

        keyPressTimer1 = 0f;
        keyPressTimer2 = 0f;
        keyPressTimer3 = 0f;

        onHoldNote = new bool[3];
    }

    void FixedUpdate()
    {
    }

    void DestroyHoldNote(int lane, int mod) // todo: fix back hold note being detected while front hold note being held
    {
        //print(lane + " " + mod);
        GameObject obj = null;

        switch (lane)
        {
            case 1:
            if (trackObj.notesLane1.Count <= 0) return; // todo: error
            trackObj.notesLane1[0].judgement -= mod;
            if (trackObj.notesLane1[0].judgement < 1) trackObj.notesLane1[0].judgement = 1;
            obj = trackObj.notesLane1[0].gameObject;
            trackObj.notesLane1.RemoveAt(0);

            break;

            case 2:
            if (trackObj.notesLane2.Count <= 0) return; // todo: error
            trackObj.notesLane2[0].judgement -= mod;
            if (trackObj.notesLane2[0].judgement < 1) trackObj.notesLane2[0].judgement = 1;
            obj = trackObj.notesLane2[0].gameObject;
            trackObj.notesLane2.RemoveAt(0);

            break;

            case 3:
            if (trackObj.notesLane3.Count <= 0) return; // todo: error
            trackObj.notesLane3[0].judgement -= mod;
            if (trackObj.notesLane3[0].judgement < 1) trackObj.notesLane3[0].judgement = 1;
            obj = trackObj.notesLane3[0].gameObject;
            trackObj.notesLane3.RemoveAt(0);

            break;
        }

        Destroy(obj);
        onHoldNote[lane - 1] = false;

        laneHold[lane] = false;
    }

    void DiffNote(int lane, int type)
    {
        //print(lane + " " + type);
        switch (lane)
        {
            case 1:
            switch (type)
            {
                case 0: // tap
                    GameObject obj = trackObj.notesLane1[0].gameObject;

                    trackObj.notesLane1.RemoveAt(0);
                    Destroy(obj);

                    player.MoveTo(1, 1);
                break;
                case 1: // hold
                    onHoldNote[0] = true;

                    player.MoveTo(1, 2);
                break;
                case 2:

                break;
            }
            break;

            case 2:
            switch (type)
            {
                case 0: // tap
                    GameObject obj = trackObj.notesLane2[0].gameObject;

                    trackObj.notesLane2.RemoveAt(0);
                    Destroy(obj);

                    player.MoveTo(2, 1);
                break;
                case 1: // hold
                    onHoldNote[1] = true;

                    player.MoveTo(2, 2);
                break;
                case 2:

                break;
            }
            break;

            case 3:
            switch (type)
            {
                case 0: // tap
                    GameObject obj = trackObj.notesLane3[0].gameObject;

                    trackObj.notesLane3.RemoveAt(0);
                    Destroy(obj);

                    player.MoveTo(3, 1);
                break;
                case 1: // hold
                    onHoldNote[2] = true;

                    player.MoveTo(3, 2);
                break;
                case 2:

                break;
            }
            break;
        }
    }

    void Judge()
    {
        // judgement

        // press note =================================================================

        if (trackObj.notesLane1.Count > 0 && (lanePress[1]))
        {
            if (trackObj.notesLane1[0].trackPos - currTime < mehThreshold)
            {
                trackObj.notesLane1[0].judgement = 1;
                if (trackObj.notesLane1[0].trackPos - currTime < greatThreshold &&
                    trackObj.notesLane1[0].trackPos - currTime > -greatThreshold)
                {
                    trackObj.notesLane1[0].judgement = 2;
                    if (trackObj.notesLane1[0].trackPos - currTime < perfectThreshold &&
                        trackObj.notesLane1[0].trackPos - currTime > -perfectThreshold)
                    {
                        trackObj.notesLane1[0].judgement = 3;
                    }
                }

                DiffNote(1, trackObj.notesLane1[0].type);
            }

            lanePress[1] = !lanePress[1];
        }
        if (trackObj.notesLane2.Count > 0 && (lanePress[2])) // for now press only
        {
            if (trackObj.notesLane2[0].trackPos - currTime < mehThreshold)
            {
                trackObj.notesLane2[0].judgement = 1;
                if (trackObj.notesLane2[0].trackPos - currTime < greatThreshold &&
                    trackObj.notesLane2[0].trackPos - currTime > -greatThreshold)
                {
                    trackObj.notesLane2[0].judgement = 2;
                    if (trackObj.notesLane2[0].trackPos - currTime < perfectThreshold &&
                        trackObj.notesLane2[0].trackPos - currTime > -perfectThreshold)
                    {
                        trackObj.notesLane2[0].judgement = 3;
                    }
                }

                DiffNote(2, trackObj.notesLane2[0].type);
            }

            lanePress[2] = !lanePress[2];
        }
        if (trackObj.notesLane3.Count > 0 && (lanePress[3])) // for now press only
        {
            if (trackObj.notesLane3[0].trackPos - currTime < mehThreshold)
            {
                trackObj.notesLane3[0].judgement = 1;
                if (trackObj.notesLane3[0].trackPos - currTime < greatThreshold &&
                    trackObj.notesLane3[0].trackPos - currTime > -greatThreshold)
                {
                    trackObj.notesLane3[0].judgement = 2;
                    if (trackObj.notesLane3[0].trackPos - currTime < perfectThreshold &&
                        trackObj.notesLane3[0].trackPos - currTime > -perfectThreshold)
                    {
                        trackObj.notesLane3[0].judgement = 3;
                    }
                }

                DiffNote(3, trackObj.notesLane3[0].type);
            }

            lanePress[3] = !lanePress[3];
        }

        // press note =================================================================

        // hold note ==================================================================

        for (int i = 1; i <= 3; i++)
        {
            switch (i)
            {
                case 1:
                if (trackObj.notesLane1.Count <= 0 || trackObj.notesLane1[0].type != 1) continue;
                break;

                case 2:
                if (trackObj.notesLane2.Count <= 0 || trackObj.notesLane2[0].type != 1) continue;
                break;

                case 3:
                if (trackObj.notesLane3.Count <= 0 || trackObj.notesLane3[0].type != 1) continue;
                break;
            }

            if (onHoldNote[i - 1])
            {
                if (laneHold[i])
                {
                    switch (i)
                    {
                        case 1:
                        if (trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime
                            < -perfectThreshold)
                            DestroyHoldNote(i, 0);
                        break;

                        case 2:
                        if (trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime
                            < -perfectThreshold)
                            DestroyHoldNote(i, 0);
                        break;

                        case 3:
                        if (trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime
                            < -perfectThreshold)
                            DestroyHoldNote(i, 0);
                        break;
                    }
                }
                else
                {
                    int judgeModify = 3;

                    switch (i)
                    {
                        case 1:
                            if (trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime < mehThreshold)
                            {
                                judgeModify = 2;
                                if (trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime < greatThreshold &&
                                    trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime > -greatThreshold)
                                {
                                    judgeModify = 1;
                                    if (trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime
                                        < perfectThreshold &&
                                        trackObj.notesLane1[0].trackPos + trackObj.notesLane1[0].trackLength - currTime
                                        > -perfectThreshold)
                                    {
                                        judgeModify = 0;
                                    }
                                }
                            }

                            DestroyHoldNote(1, judgeModify);

                            break;
                        case 2:
                            if (trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime < mehThreshold)
                            {
                                judgeModify = 2;
                                if (trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime < greatThreshold &&
                                    trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime > -greatThreshold)
                                {
                                    judgeModify = 1;
                                    if (trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime
                                        < perfectThreshold &&
                                        trackObj.notesLane2[0].trackPos + trackObj.notesLane2[0].trackLength - currTime
                                        > -perfectThreshold)
                                    {
                                        judgeModify = 0;
                                    }
                                }
                            }

                            DestroyHoldNote(2, judgeModify);

                            break;
                        case 3:
                            if (trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime < mehThreshold)
                            {
                                judgeModify = 2;
                                if (trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime < greatThreshold &&
                                    trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime > -greatThreshold)
                                {
                                    judgeModify = 1;
                                    if (trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime
                                        < perfectThreshold &&
                                        trackObj.notesLane3[0].trackPos + trackObj.notesLane3[0].trackLength - currTime
                                        > -perfectThreshold)
                                    {
                                        judgeModify = 0;
                                    }
                                }
                            }

                            DestroyHoldNote(3, judgeModify);

                            break;
                    }
                }
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        currTime = trackObj.audioManager.music[0].source.time; // current time

        // Debug.Log("======");
        // for (int i = 1; i < 4; ++i)
        // {
        //     Debug.Log("Pressed " + i + " : " + lanePress[i].ToString());
        //     Debug.Log("Held " + i + " : " + laneHold[i].ToString());
        // }

        if (!trackObj.audioManager.music[0].source.isPlaying) return;

        Detect();
        
        Judge();

        //if (!trackObj.audioManager.sounds[0].source.isPlaying) return;

        if ((lanePress[1] || laneHold[1]) && !pSys1.activeSelf) pSys1.SetActive(true);
        else if (!(lanePress[1] || laneHold[1]) && pSys1.activeSelf) pSys1.SetActive(false);

        if ((lanePress[2] || laneHold[2]) && !pSys2.activeSelf) pSys2.SetActive(true);
        else if (!(lanePress[2] || laneHold[2]) && pSys2.activeSelf) pSys2.SetActive(false);

        if ((lanePress[3] || laneHold[3]) && !pSys3.activeSelf) pSys3.SetActive(true);
        else if (!(lanePress[3] || laneHold[3]) && pSys3.activeSelf) pSys3.SetActive(false);
    }

    void Detect()
    {
        // inputs

        // lane 1 ========================================
        if (Input.GetKeyDown(lane1Key))
        {
            keyPressTimer1 = pressThreshold;

            lanePress[1] = true;
        }

        if (Input.GetKey(lane1Key)) laneHold[1] = true;
        else
        {
            laneHold[1] = false;
            onHoldNote[0] = false;
        }

        // ===============================================

        // lane 2 ========================================
        if (Input.GetKeyDown(lane2Key)) 
        {
            keyPressTimer2 = pressThreshold;

            lanePress[2] = true;
        }

        if (Input.GetKey(lane2Key)) laneHold[2] = true;
        else 
        {
            laneHold[2] = false;
            onHoldNote[1] = false;
        }

        // ===============================================

        // lane 3 ========================================
        if (Input.GetKeyDown(lane3Key)) 
        {
            keyPressTimer3 = pressThreshold;

            lanePress[3] = true;
        }

        if (Input.GetKey(lane3Key)) laneHold[3] = true;
        else
        {
            laneHold[3] = false;
            onHoldNote[2] = false;
        }


        // timer for key press buffer
        if (lanePress[1]) // lane 1
        {
            keyPressTimer1 -= Time.deltaTime;
            if (keyPressTimer1 <= 0f) lanePress[1] = false;
        }

        if (lanePress[2]) // lane 2
        {
            keyPressTimer2 -= Time.deltaTime;
            if (keyPressTimer2 <= 0f) lanePress[2] = false;
        }

        if (lanePress[3]) // lane 3
        {
            keyPressTimer3 -= Time.deltaTime;
            if (keyPressTimer3 <= 0f) lanePress[3] = false;
        }

        // ===============================================

        // movement

        for (int i = 1; i < 4; i++)
        {
            //print(i + " " + lanePress[i] + " " + laneHold[i]);
            if (lanePress[i])
            {
                //player.Reset();
                player.MoveTo(i, 0);
            }

            if (laneHold[i])
            {
                player.GetComponent<Rigidbody2D>().constraints |= RigidbodyConstraints2D.FreezePositionY;

                break;
            }
        }

        //print(player.state);

        // todo: fix (constraints stuck)
        if (!laneHold[1] && !laneHold[2] && !laneHold[3] && 
            player.state != PlayerController.State.idle && player.state != PlayerController.State.fall)
        {
            player.GetComponent<Rigidbody2D>().constraints &= ~RigidbodyConstraints2D.FreezePositionY;
            player.MoveTo(0, 0);
        }
    }
}
