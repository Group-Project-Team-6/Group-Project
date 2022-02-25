using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StickySplash : MonoBehaviour
{
    float life = 1.0f;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        life -= Time.deltaTime;
        transform.position -= new Vector3(0,Time.deltaTime * 0.3f,0);
        if (life <= 0.001f)
        {
            Destroy(gameObject);
        }
        Color c = transform.GetComponent<Renderer>().material.color;
        transform.GetComponent<Renderer>().material.color = new Color(c.r, c.g, c.b, life );

    }
}
