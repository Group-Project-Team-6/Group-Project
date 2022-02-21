using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;


public class Score : MonoBehaviour
{
    public int score = 1;
    public UnityEvent onCollect;

    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
       // if (Collsion)
        //Invoke
        //Destroy(gameObject);
    }
    void OnTriggerEnter()
    {
        Destroy(gameObject);
        
    }
}
