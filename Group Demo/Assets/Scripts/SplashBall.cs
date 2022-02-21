using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashBall : MonoBehaviour
{
    public float life = 1.0f;
    public float mass = 1.0f;
    public float radius = 1.0f;
    public Color32 color = Color.black;
    public Texture2D tex;
    // Start is called before the first frame update
    void Awake()
    {

    }

    // Update is called once per frame
    void Update()
    {
        life -= Time.deltaTime;
    }

    void OnCollisionEnter(Collision collision)
    {
        Splash(collision);
    }

    void OnCollisionStay(Collision collision)
    {
        Splash(collision);
    }

    private void Splash(Collision collision)
    {
        if (collision.gameObject.GetComponent<SplashBall>()) return;
        //if (life > 0.0f)
        //{
        //    life *= 0.5f;
        //    return;
        //}
        SplashOut(collision);
        Destroy(gameObject);
    }

    private void SplashOut(Collision collision)
    {
        if (radius > 0.25f)
        {
            Vector3 normal = collision.contacts[0].normal;
            Vector3 pt = transform.position + collision.contacts[0].separation * normal.normalized;
            Vector3 u = this.GetComponent<Rigidbody>().velocity;
            for (int i = 0; i < 4; i++)
            {
                GameObject newSplash = Instantiate(
                    gameObject,
                    pt,
                    new Quaternion(),
                    this.transform.parent);
                Vector3 dir = normal.normalized +
                    new Vector3(
                        Random.Range(-1.0f, 1.0f),
                        Random.Range(-1.0f, 1.0f),
                        Random.Range(-1.0f, 1.0f)
                        );
                
                newSplash.GetComponent<Rigidbody>().velocity = 
                    (this.GetComponent<Rigidbody>().velocity.normalized + dir).normalized
                    * this.GetComponent<Rigidbody>().velocity.magnitude * 0.5f;
                SplashBall subSplashBall = newSplash.GetComponent<SplashBall>();
                subSplashBall.radius = radius * 0.5f;
                subSplashBall.mass = mass * 0.5f;
                subSplashBall.color = color;
                subSplashBall.tex = tex;

            }
        }
    }
}
