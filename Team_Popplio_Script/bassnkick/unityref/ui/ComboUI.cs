using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ComboUI : MonoBehaviour
{
    [SerializeField]
    TMPro.TMP_Text text;
    [SerializeField]
    Score score;

    [SerializeField]
    Color color;

    float animTimer = 0f;
    float animSpeed = 5f;

    [SerializeField]
    uint currCombo = 0;

    float fontSize = 1f;

    // Start is called before the first frame update
    void Start()
    {
        fontSize = text.fontSize;
    }

    void Update()
    {
        if (score.currCombo == 0)
        {
            if (score.currCombo != currCombo)
            {
                currCombo = score.currCombo;
                text.text = "";
            }
            return;
        }

        if (score.currCombo == currCombo && animTimer == 0f)
        {
            return;
        }

        if (score.currCombo != currCombo)
        {
            currCombo = score.currCombo;
            if (currCombo > 0) text.text = currCombo.ToString();
            else text.text = "";
        }

        // timer
        animTimer += Time.deltaTime * animSpeed;

        // text animation
        //tmp.fontSize = easeOutElastic(animTimer);
        text.fontSize = fontSize * easeOutElastic(animTimer);

        if (animTimer >= 1f) 
        {
            animTimer = 0f;
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
