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

        if (GameManager.twinningState == GameManager.WinningState.TEAM1)
        {
            text.text = "Team 1 Win";
        }else if (GameManager.twinningState == GameManager.WinningState.TEAM2)
        {
            text.text = "Team 2 Win";
        }
        else
        {
            text.text = "Draw";
        }
    }
}
