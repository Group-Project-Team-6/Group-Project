using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class InputManager : MonoBehaviour
{

    public GameObject g;
    bool EscToggle = false;
    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            EscToggle = !EscToggle;
            Debug.Log("Escape button pressed");
            if (EscToggle)
            {
                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
                g.SetActive(true);
            }
            else
            {
                ResumeGame();
            }
            
        }


    }

    public void ResumeGame()
    {
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
        g.SetActive(false);
    }
}
