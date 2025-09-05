using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class NoteFeedback : MonoBehaviour
{
    [Header("Fields")]

    [SerializeField]
    Sprite miss;
    [SerializeField]
    Sprite bad;
    [SerializeField]
    Sprite nice; 
    [SerializeField]
    Sprite perfect;
    [SerializeField]
    GameObject text;
    //GameObject text;
    public Canvas canvas;

    [Header("Settings")]

    [SerializeField]
    float animSpeed = 1f;
    [SerializeField]
    float textSpeed = .02f;

    [Header("State")]
    public string judge;

    float animTimer = 0f;

    //TMP_Text tmp; // text

    void OnEnable()
    {
        // put under canvas
        text.transform.SetParent(canvas.transform);
        text.transform.localScale = new Vector3(1f, 1f, 1f);

        //tmp = text.GetComponent<TMP_Text>(); // text

        // color assign
        if (judge == "miss")
        {
            //tmp.color = Color.red;
            GetComponent<SpriteRenderer>().color = Color.grey;
            text.GetComponent<SpriteRenderer>().sprite = miss;
        }
        else if (judge == "meh")
        {
            //tmp.color = Color.green;
            GetComponent<SpriteRenderer>().color = Color.grey;
            text.GetComponent<SpriteRenderer>().sprite = bad;
        }
        else if (judge == "great")
        {
            //tmp.color = Color.blue;
            GetComponent<SpriteRenderer>().color = Color.yellow;
            text.GetComponent<SpriteRenderer>().sprite = nice;
        }
        else if (judge == "perfect")
        {
            //tmp.color = Color.yellow;
            GetComponent<SpriteRenderer>().color = Color.magenta;
            text.GetComponent<SpriteRenderer>().sprite = perfect;
        }
        // text assign
        //tmp.text = judge;
    }

    // Update is called once per frame
    void Update()
    {
        // timer
        animTimer += Time.deltaTime * animSpeed; 

        // text animation
        //tmp.fontSize = easeOutElastic(animTimer);
        text.transform.localScale = new Vector3(1, 1, 1) * easeOutElastic(animTimer);
        text.transform.Translate(Vector3.up * textSpeed);

        // sprite animation
        transform.localScale = new Vector3(1, 1, 1) * animTimer * 2f;

        // color animation
        Color temp = GetComponent<SpriteRenderer>().color;
        temp.a = 1f - animTimer;
        GetComponent<SpriteRenderer>().color = temp;

        //tmp.color = temp;
        temp = text.GetComponent<SpriteRenderer>().color;
        temp.a = 1f - animTimer;
        text.GetComponent<SpriteRenderer>().color = temp;

        // destroy when animation ends
        if (GetComponent<SpriteRenderer>().color.a <= 0)
        {
            //Destroy(tmp.gameObject);
            Destroy(text);
            Destroy(this.gameObject);
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
