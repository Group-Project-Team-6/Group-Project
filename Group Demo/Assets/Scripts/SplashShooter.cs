using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashShooter : MonoBehaviour
{
    public GameObject obj;
    public GameObject splashes;
    public PlayerController pController;
    public float dist;
    public Camera cam;
    public Color32 color;
    public List<Texture2D> SplashTexs;
    public int index = 0;
    public float shotHeight;
    public bool shoot;
    public float spread;
    int count = 0;

    float delay = 0;
    // Start is called before the first frame update
    private void Awake()
    {
        pController.OnMouseEvent += OnMouseEvent;
    }
    // Update is called once per frame
    void Update()
    {
        delay += Time.deltaTime;
        if (!cam) return;
        if (shoot)
        {
            if (delay > 0.3f)
            {
                index++;
                if (index >= SplashTexs.Count) index = 0;
                GameObject g = Instantiate(obj, transform.position + transform.forward * dist, new Quaternion());
                if (g.GetComponent<Rigidbody>()) g.GetComponent<Rigidbody>().AddForce((transform.position - cam.transform.position + transform.up * shotHeight).normalized * dist * 1000);
                if (g.GetComponent<SplashBall>())
                {
                    SplashBall sb = g.GetComponent<SplashBall>();
                    sb.team = gameObject.layer - 9;
                    sb.color = this.color;
                    sb.mass = 1;
                    sb.radius = 1;
                    sb.tex = SplashTexs[index];
                }
                delay = 0;
                count = 0;
            }

            if(count < 3)
            {
                for (int i = 0; i < 10; i++)
                {
                    Vector3 bias = new Vector3(Random.Range(-spread, spread), Random.Range(-spread, spread), Random.Range(-spread, spread));
                    GameObject s = Instantiate(splashes, transform.position + transform.forward * dist, new Quaternion());
                    Vector3 v = (transform.position - cam.transform.position + transform.up * shotHeight).normalized;
                    float ss = spread * 0.1f;
                    if (s.GetComponent<Rigidbody>()) s.GetComponent<Rigidbody>().AddForce(new Vector3(v.x + Random.Range(-ss, ss), v.y + Random.Range(-ss, ss), v.z + Random.Range(-ss, ss)).normalized * dist * 1000);
                }
            }
            count++;
        }
    }

    public void OnMouseEvent(object sender, MouseArgs args)
    {
        shoot = args.shoot;
    }
}
