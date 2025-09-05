using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fever : MonoBehaviour
{
    [Header("Fever Feedback Settings")]
    [SerializeField]
    float duration = .4f;
    [SerializeField]
    float variation = .05f;
    [SerializeField]
    float alphaModifier = .7f;

    // when to play feedback
    bool playFeedback = false;

    // timer for animation
    float timer = 0f;

    SpriteRenderer sprite; // sprite

    float r = .7f, g = .7f, b = .7f, a = 0f; // color properties

    // Start is called before the first frame update
    void Start()
    {
        // initialize
        r = g = b = .7f; 
        a = 0f;

        sprite = GetComponent<SpriteRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        if (!playFeedback) return; // don't play if not activated

        timer += Time.deltaTime; // timer

        // randomise color
        r = Mathf.Clamp(Random.Range(r - variation, r + variation), .5f, .95f);
        g = Mathf.Clamp(Random.Range(g - variation, g + variation), .5f, .95f);
        b = Mathf.Clamp(Random.Range(b - variation, b + variation), .5f, .95f);
        a = (-4f * Mathf.Pow(timer / duration, 2f)) + (4 * (timer / duration)) * alphaModifier;

        sprite.color = new Color(r, g, b, a);

        if (timer > duration) // end animation
        {
            r = g = b = .7f;
            a = 0f;
            timer = 0f;
            
            playFeedback = false;
        }
    }

    public void feedback() // external function
    {
        playFeedback = true;
    }
}
