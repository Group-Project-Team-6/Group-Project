using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class ScoreArgs : EventArgs
{
    public GameObject player;
}


public class Score : MonoBehaviour
{
    public event EventHandler<ScoreArgs> onCollect;
    
    //public UnityEvent onCollect;
    

    void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.layer == 10)
        {
            GameManager.gameManager.Team1Score++;
            //Change HUD Data (Gokul)
        }

        else if (other.gameObject.layer == 11) 
        { 
            GameManager.gameManager.Team2Score++;
            //Change HUD Data (Gokul)
        }

        ScoreArgs scoreArgs = new ScoreArgs();
        scoreArgs.player = other.gameObject;
        onCollect.Invoke(this, scoreArgs);
        Destroy(gameObject);        
    }
}
