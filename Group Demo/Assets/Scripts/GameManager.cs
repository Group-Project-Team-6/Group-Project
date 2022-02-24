using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public List<GameObject> players;
    public static GameManager gameManager;
    public CollectableGenerator collectableGenerator;

    public float gameTime;
    public int Team1Score;
    public int Team2Score;
    public static bool team1Win;
    int maxScore;
    const int size = 20;
    public Score[] scores = new Score[size];

    int maxPoints; //Needs Gets Value from Collectable Script

    void Awake()
    {
        if (gameManager == null)
        {
            gameManager = this;
            players = new List<GameObject>();
        }
        else
        {
            print("Duplicated GameManager");
        }

    }

    void Start()
    {
        gameTime = 30;

        maxScore = CollectableGenerator.numCollectables;
    }

    // Update is called once per frame
    void Update()
    {
        gameTime -= Time.deltaTime;

        if (gameTime <= 0)
        {
            EndGame();
        }
    }

    public void GivePoints(object sender, ScoreArgs scoreArgs)
    {
        Debug.Log("Event Triggered");

        if (scoreArgs.player.gameObject.layer == 10)
        {
            GameManager.gameManager.Team1Score++;
            //Change HUD Data (Gokul)
        }

        else if (scoreArgs.player.gameObject.layer == 11)
        {
            GameManager.gameManager.Team2Score++;
            //Change HUD Data (Gokul)
        }

        if (Team1Score > ((maxScore / 2) + 1) || Team2Score > ((maxScore / 2) + 1))
        {
            EndGame();
        }

    }

    void EndGame()
    {
        if (Team1Score > Team2Score)
        {
            team1Win = true;
        }
        else team1Win = false;

        SceneManager.LoadScene("EndScene");

    }
}
