using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetUI : MonoBehaviour
{
    float timer = 0f;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        RectTransform rectT = GetComponent<RectTransform>();
        rectT.eulerAngles = 
            new Vector3(rectT.eulerAngles.x, rectT.eulerAngles.y, timer += Time.deltaTime * 20f);
    }
}
