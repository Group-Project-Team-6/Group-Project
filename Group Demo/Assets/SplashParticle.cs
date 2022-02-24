using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashParticle : MonoBehaviour
{
    short life = 10;
    // Start is called before the first frame update
    void OnCollisionEnter(Collision collision)
    {
        if (life <= 0)
            Destroy(gameObject);
        else
        {
            life--;
        }
    }

    void OnCollisionStay(Collision collision)
    {
        if (life <= 0)
            Destroy(gameObject);
        else
        {
            life--;
        }
    }
}
