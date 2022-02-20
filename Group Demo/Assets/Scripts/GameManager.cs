using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;
    public float gameTime;
    public int Team1Score;
    public int Team2Score;

    public bool team1Win;

    int maxPoints;

    void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            print("Duplicated GameManager");
        }
    }

    void Start()
    {
        gameTime = 300;

        var score = GetComponent<Score>();
        score.onCollect.AddListener(GivePoints);
    }

    // Update is called once per frame
    void Update()
    {
        gameTime -= Time.deltaTime;
        print("GameTime " + gameTime); 

        if (gameTime <= 0)
        {
            //Team1Score > Team2Score
            //Bool Team1Win = true;
            //else Team1Win = false
            EndGame();
        }
    }

    void GivePoints()
    {
        //if Team1 Layer Collision
        Team1Score++;
        //Change HUD Data

        //else Team2 Layer Collision
        Team2Score++;
        //Change HUD Data

        if(Team1Score > (maxPoints/2 + 1))
        {
            EndGame(); //Pass Bool  True
        }
        
        if( Team2Score > (maxPoints / 2 + 1))
        {
            EndGame(); //Pass Bool False
        }
    }

    void EndGame()
    {
        //Load End Scene
    }
}
