using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager gameManager;
    public CollectableGenerator collectableGenerator;

    public float gameTime;
    public int Team1Score;
    public int Team2Score;
    public bool team1Win;
    int maxScore;
    const int size = 20;
    public Score[] scores = new Score[size];

    int maxPoints; //Needs Gets Value from Collectable Script

    void Awake()
    {
        if (gameManager == null)
        {
            gameManager = this;
        }
        else
        {
            print("Duplicated GameManager");
        }

    }

    void Start()
    {
        gameTime = 300;

        maxScore = CollectableGenerator.numCollectables;
    }

    // Update is called once per frame
    void Update()
    {
        gameTime -= Time.deltaTime;

        if (gameTime <= 0)
        {
            if (Team1Score > Team2Score)
            {
                team1Win = true;
            }

            else team1Win = false;

            EndGame(ref team1Win);
        }
    }

    public void GivePoints()
    {
        Debug.Log("Event Triggered");

        if(Team1Score > (maxScore / 2 + 1))
        {
            team1Win = true;
            EndGame(ref team1Win);
        }
        
        if( Team2Score > (maxScore / 2 + 1))
        {
            team1Win = false;
            EndGame(ref team1Win);
        }
    }

    void EndGame(ref bool gameResult)
    {

        if (gameResult)
        {
            //team1Win Result Code (Gokul) 
        }

        if (!gameResult)
        {
            //team2Win Result Code (Gokul)
        }
    }
}
