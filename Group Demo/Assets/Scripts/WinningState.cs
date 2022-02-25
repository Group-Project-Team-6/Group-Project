using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class WinningState : MonoBehaviour
{
    public void OnWinButtonClick()
    {
        SceneManager.LoadScene(0);
    }
}
