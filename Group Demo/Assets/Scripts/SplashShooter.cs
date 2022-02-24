using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashShooter : MonoBehaviour
{
    public GameObject obj;
    public PlayerController pController;
    public float dist;
    public Camera cam;
    public Color32 color;
    public List<Texture2D> SplashTexs;
    public int index = 0;
    public float shotHeight;
    public bool shoot;

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
        if (shoot && (delay > 0.3f))
        {

            index++;
            if (index >= SplashTexs.Count) index = 0;
            GameObject g = Instantiate(obj, transform.position + transform.forward * dist, new Quaternion());
            if (g.GetComponent<Rigidbody>()) g.GetComponent<Rigidbody>().AddForce((transform.position - cam.transform.position + transform.up * shotHeight).normalized * dist * 1000);
            if (g.GetComponent<SplashBall>()) { 
                SplashBall sb = g.GetComponent<SplashBall>();
                sb.team = gameObject.layer - 9;
                sb.color = this.color;
                sb.mass = 1;
                sb.radius = 1;
                sb.tex = SplashTexs[index];
            }
            delay = 0;
        }
    }

    public void OnMouseEvent(object sender, MouseArgs args)
    {
        shoot = args.shoot;
    }
}
