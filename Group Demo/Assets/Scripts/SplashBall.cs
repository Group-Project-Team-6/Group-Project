using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashBall : MonoBehaviour
{
    public float mass = 1.0f;
    public float radius = 1.0f;
    public Color32 color = Color.black;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnCollisionEnter(Collision collision)
    {
        Destroy(gameObject);
    }

    void OnCollisionStay(Collision collision)
    {
        Destroy(gameObject);
    }
}
