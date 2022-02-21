using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public int Team;
    public int Health;
    public PlayerController pc;
    Renderer render;

    void Awake()
    {
       // pc = this.GetComponent<PlayerController>();
        render = this.GetComponent<Renderer>();
    }

    // Start is called before the first frame update
    void Start()
    {
        Health = 3; 
    }

    // Update is called once per frame
    void Update()
    {
        if (Health == 0) {
           // pc.fainted = true;
            render.materials[0].color = new Color(1,0,1,1);
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.GetComponent<SplashBall>())
        {
            if (Health > 0)
            {
                Health--;
            }
        }
    }
}
