using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameSystem : MonoBehaviour
{
    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1)) SceneManager.LoadScene("Main");
        if (Input.GetKeyDown(KeyCode.Alpha2)) SceneManager.LoadScene("Tutorial");
        if (Input.GetKeyDown(KeyCode.Alpha3)) SceneManager.LoadScene("Results");
    }
}
