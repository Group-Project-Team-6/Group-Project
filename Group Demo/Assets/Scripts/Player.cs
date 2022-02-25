using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public int Team;
    public int Health = 3;
    public int maxHealth = 3;
    public PlayerController pc;
    Renderer render;
    float counter;
    void Awake()
    {
        render = this.GetComponent<Renderer>();
        pc = GetComponent<PlayerController>();
    }

    void Start()
    {
        Revive();
    }

    void Update()
    {
        if (Health <= 0)
        {
            if(pc.fainted == false)
            {
                pc.fainted = true;
                render.materials[0].color = new Color(1, 0, 1, 0.3f);
                counter = 5.0f;
            }
            if (counter < 0.0f)
            {
                Revive();
            }
            counter -= Time.deltaTime;
        }
    }

    public void Revive()
    {
        Health = maxHealth;
        pc.fainted = false;
        render.materials[0].color = new Color(1, 1, 1, 1.0f);
        counter = 0;
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.GetComponent<SplashBall>())
        {
            Debug.Log("Team: " + collision.gameObject.GetComponent<SplashBall>().Team + " - " + Team);
            if (collision.gameObject.GetComponent<SplashBall>().Team != Team)
            {
                if (Health > 0)
                {
                    Health--;
                }
            }
            else if (collision.gameObject.GetComponent<SplashBall>().Team == Team)
            {
                
                if (Health < maxHealth)
                {
                    Health++;
                }
                
            }
            
        }
    }
}
