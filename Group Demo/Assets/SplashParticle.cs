using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashParticle : MonoBehaviour
{
    public GameObject stickySplash;
    public float spread;
    short life = 3;
    // Start is called before the first frame update
    void OnCollisionEnter(Collision collision)
    {
        if (life <= 0)
        {
            for (int i = 0; i < 10; i++)
            {
                Instantiate(stickySplash);
            }
            Destroy(gameObject);
        }
        else
        {
            life--;
        }
    }

    void OnCollisionStay(Collision collision)
    {
        if (life <= 0)
        {
            Vector3 bias = (new Vector3(Random.Range(-spread, spread), Random.Range(-spread, spread), Random.Range(-spread, spread))) ;
            bias -= Vector3.Dot(bias, collision.contacts[0].normal) * collision.contacts[0].normal;
            Instantiate(
                stickySplash,
                collision.contacts[0].point + bias,
                new Quaternion());
            Destroy(gameObject);
        }
        else
        {
            life--;
        }
    }
}
