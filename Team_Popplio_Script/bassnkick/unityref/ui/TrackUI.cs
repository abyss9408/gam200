using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Mathematics;
using System;

public class TrackUI : MonoBehaviour
{
    [SerializeField]
    GameObject marker;
    [SerializeField]
    GameObject bar;

    [SerializeField]
    Track track;

    float animTimer = 0f;

    float animSpeed = 3f;
    float animScale = 15f;

    Transform markerT;

    // Start is called before the first frame update
    void Start()
    {
        markerT = marker.transform;
    }

    // Update is called once per frame
    void Update()
    {
        RectTransform rectT = bar.GetComponent<RectTransform>();
        RectTransform markerRectT = marker.GetComponent<RectTransform>();
        markerRectT.position = new Vector3(rectT.position.x - 2.75f + 
            (5.5f * track.audioManager.music[0].source.time / track.audioManager.music[0].clip.length), 
            markerRectT.position.y, markerRectT.position.z);

        // timer
        animTimer += Time.deltaTime * animSpeed;

        // text animation
        //tmp.fontSize = easeOutElastic(animTimer);
        markerRectT.eulerAngles = new Vector3(0f, 0f, animScale * (float)Math.Cos(Math.PI * animTimer));
    }
}
