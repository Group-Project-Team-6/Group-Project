using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;


public class Score : MonoBehaviour
{
    public int score = 1;
    public UnityEvent onCollect;

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

        onCollect.Invoke();
        Destroy(gameObject);        
    }
}
