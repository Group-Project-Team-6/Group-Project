using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public Camera cam;
    public int Team;
    public int Health;
    PlayerController pc;
    Renderer render;

    void Awake()
    {
        render = this.GetComponent<Renderer>();
    }

    void Start()
    {
        Health = 3;
    }

    void Update()
    {
        if (Health == 0)
        {
            // pc.fainted = true;
            render.materials[0].color = new Color(1, 0, 1, 1);
        }
        else 
        {
            //pc.fainted = false;
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.GetComponent<SplashBall>())
        {   
            if (collision.gameObject.GetComponent<SplashBall>().Team != Team)
            {
                if (Health > 0)
                {
                    Health--;
                }
            }
            else if (collision.gameObject.GetComponent<SplashBall>().Team == Team)
            {
                
                if (Health < 3)
                {
                    Health++;
                }
                
            }
            
        }
    }
}
