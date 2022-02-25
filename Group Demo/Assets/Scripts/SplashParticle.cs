using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashParticle : MonoBehaviour
{
    public GameObject stickySplash;
    public float spread;
    public Color color;
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
            Vector3 bias = (new Vector3(Random.Range(-spread, spread), Random.Range(-spread, spread), Random.Range(-spread, spread)));
            bias -= Vector3.Dot(bias, collision.contacts[0].normal) * collision.contacts[0].normal;
            GameObject g = Instantiate(
                stickySplash,
                collision.contacts[0].point + bias,
                new Quaternion());
            g.GetComponent<Renderer>().material.SetColor("_Color", this.color); 
            Destroy(gameObject);
        }
        else
        {
            life--;
        }
    }
}
