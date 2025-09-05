using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Results : MonoBehaviour
{
    Score score;

    float timer = 0f;

    float animSpeed = 1f;

    [SerializeField]
    List<TMP_Text> results = new List<TMP_Text>();
    List<float> resultVals = new List<float>();

    float defaultFontSize = 28f;

    // Start is called before the first frame update
    void Start()
    {
        score = FindObjectOfType<Score>();

        foreach (TMP_Text text in results) text.text = "0";

        resultVals.Add(score.currMaxCombo);
        resultVals.Add(score.currScore); // convert to percentage of max score in M3
        resultVals.Add(score.miss);
        resultVals.Add(score.bad);
        resultVals.Add(score.nice);
        resultVals.Add(score.perfect);
    }

    // Update is called once per frame
    void Update()
    {
        timer += Time.deltaTime * animSpeed;

        if (timer > 1f) timer = 1f;

        for (int i = 0; i < resultVals.Count; ++i)
        {
            if (i == 0) defaultFontSize = 40f;
            else if (i == 1) defaultFontSize = 45f;
            else defaultFontSize = 28f;

            results[i].fontSize = defaultFontSize * easeOutElastic(timer);

            results[i].text = ((int)(resultVals[i] * timer)).ToString();
        }
    }

    // to animate feedback
    float easeOutElastic(float x)
    {
        if (x > 1f) return 1f;
        else if (x < 0f) return 0f;

        float c4 = (2f * Mathf.PI) / 3f;

        return Mathf.Pow(2f, -10f * x) * Mathf.Sin((x * 10f - 0.75f) * c4) + 1f;
    }
}
