using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class GameManager : MonoBehaviour
{
    public enum WinningState
    {
        TEAM1,
        TEAM2,
        DRAW
    };
    public List<GameObject> players;
    public static GameManager gameManager;
    public CollectableGenerator collectableGenerator;

    public float gameTime;
    public int Team1Score;
    public int Team2Score;
    public static WinningState twinningState;
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
        ResetDefault(300);
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

    public void ResetDefault(float gameTime)
    {
        this.gameTime = gameTime;
        maxScore = CollectableGenerator.numCollectables;
        Team1Score = 0;
        Team2Score = 0;
        twinningState = WinningState.DRAW;
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
            twinningState = WinningState.TEAM1;
        }
        else if (Team1Score < Team2Score)
        {
            twinningState = WinningState.TEAM2;
        }
        else
        {
            twinningState = WinningState.DRAW;
        }
        Cursor.lockState = CursorLockMode.None;
        Cursor.visible = true;
        SceneManager.LoadScene("EndScene");

    }
}
