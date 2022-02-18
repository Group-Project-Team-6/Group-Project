using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public float gameTime;
    public int Score;

    // Start is called before the first frame update
    void Start()
    {
        //Get Player Data
        gameTime = 300;
    }

    // Update is called once per frame
    void Update()
    {
        //Subtract tick from timer
        //Update Timer;   

        if (Timer <= 0)
        {
            //End Game Function
        }
    }

    //void OnEventScore()
        //if
        //Get items in level
        //Call EndGame

    void EndGame()
    {

    }
}
