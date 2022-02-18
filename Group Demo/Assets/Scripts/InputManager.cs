using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class InputManager : MonoBehaviour
{

    public GameObject g;

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            Debug.Log("Escape button pressed");
            g.SetActive(true);
        }


    }

    public void ResumeGame()
    {
        g.SetActive(false);
    }
}
