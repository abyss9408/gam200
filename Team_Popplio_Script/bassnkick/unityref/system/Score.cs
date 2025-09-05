using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Score : MonoBehaviour
{
    
    [SerializeField]
    Track track;

    public uint maxCombo = 0; // theoretical max
    public uint currCombo = 0;
    public uint currMaxCombo = 0;

    public uint maxScore = 0; // theoretical max
    public uint currScore = 0;

    public uint miss = 0;
    public uint bad = 0;
    public uint nice = 0;
    public uint perfect = 0;

    // Start is called before the first frame update
    void Start()
    {
        DontDestroyOnLoad(this.gameObject);
        currCombo = 0;
        currMaxCombo = 0;

        currScore = 0;

        maxCombo = (uint)(track.notesLane1.Count + track.notesLane2.Count + track.notesLane3.Count);
        maxScore = maxCombo * 3;
    }

    // Update is called once per frame
    void Update()
    {
        if (SceneManager.GetActiveScene() == SceneManager.GetSceneByName("Results"))
        {

        }
    }

    public void UpdateCombo(bool b)
    {
        if (b) ++currCombo; 
        else currCombo = 0;

        if (currCombo > currMaxCombo) currMaxCombo = currCombo;
    }

    public void UpdateScore(uint s)
    {
        switch (s)
        {
            case 0:
                ++miss;
                UpdateCombo(false);
            break;

            case 1:
                ++bad;
                ++currScore;
                UpdateCombo(true);
            break;
            case 2: 
                ++nice;
                currScore += 2;
                UpdateCombo(true);
            break;
            case 3:
                ++perfect;
                currScore += 3;
                UpdateCombo(true);
            break;
        }
    }
}
