using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class InputManager : MonoBehaviour
{

    public GameObject g;
    static bool EscToggle = false;

    // Update is called once per frame
    private void Awake()
    {
       
    }
    void Update()
    {
        Debug.Log(Time.timeScale);
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            ResumeGame();            
        }


    }

    public void ResumeGame()
    {
        EscToggle = !EscToggle;
        Debug.Log("Escape button pressed");
        if (EscToggle)
        {
            Time.timeScale = 0.0f;
            Cursor.lockState = CursorLockMode.None;
            Cursor.visible = true;
            g.SetActive(true);
        }
        else
        {
            Time.timeScale = 1.0f;
            Cursor.lockState = CursorLockMode.Locked;
            Cursor.visible = false;
            g.SetActive(false);
        }
    }
}
