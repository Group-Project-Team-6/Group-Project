using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreDisplay : MonoBehaviour
{
    public int TeamNum;
    TMPro.TextMeshProUGUI tmp;
    // Start is called before the first frame update
    void Start()
    {
        tmp = gameObject.GetComponent<TMPro.TextMeshProUGUI>();

    }

    // Update is called once per frame
    void Update()
    {
        int score;
        if (TeamNum == 1) score = GameManager.gameManager.Team1Score;
        else score = GameManager.gameManager.Team2Score;
        tmp.text = "Team " + TeamNum + ":\n" + score;
    }
}
