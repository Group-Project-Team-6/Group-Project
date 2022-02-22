using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager gameManager;
    public CollectableGenerator collectableGenerator;
    public GameObject collectableGen;

    public float gameTime;
    public int Team1Score;
    public int Team2Score;
    public bool team1Win;

    int maxPoints; //Needs Gets Value from Collectable Script

    public Score[] scores;

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

        //scores = GetComponents<Score>(collectableGenerator.);
        scores[0].onCollect.AddListener(GivePoints);

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

    void GivePoints()
    {
        Debug.Log("Event Triggered");

        //if Team1 Layer Collision
        Team1Score++;
        //Change HUD Data (Gokul)

        //else Team2 Layer Collision
        Team2Score++;
        //Change HUD Data (Gokul)

        if(Team1Score > (maxPoints/2 + 1))
        {
            team1Win = true;
            EndGame(ref team1Win);
        }
        
        if( Team2Score > (maxPoints / 2 + 1))
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
