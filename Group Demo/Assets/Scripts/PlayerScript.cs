using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerScript : MonoBehaviour
{
    public int life = 3;

    // Start is called before the first frame update
    void Start()
    {

    }

    void OnCollisionEnter(Collision collision)
    {
        // Destroy(gameObject other);
        life--;

        if(life <= 0)
        {
            //Disable player controls
            //Start Timer
        }
    }
}
