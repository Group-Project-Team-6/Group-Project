using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class WinText : MonoBehaviour
{
    Text text;

    private void Awake()
    {
        text = GetComponent<Text>();

        if (GameManager.team1Win)
        {
            text.text = "Team 1 Win";
        }

        else text.text = "Team 2 Win";
    }
}
