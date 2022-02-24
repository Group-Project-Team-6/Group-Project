using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;



public class ScoreArgs : EventArgs
{
    public GameObject player;
    public int eventScore;
}

public class Score : MonoBehaviour
{
    public event EventHandler<ScoreArgs> OnCollect;
    public int score;

    void Awake()
    {
        OnCollect += GameManager.gameManager.GivePoints;
    }
    void OnTriggerEnter(Collider other)
    {
        ScoreArgs scoreArgs = new ScoreArgs();
        scoreArgs.player = other.gameObject;
        scoreArgs.eventScore = score;
        OnCollect.Invoke(this, scoreArgs);
        Destroy(gameObject);
    }
}
