using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashShooter : MonoBehaviour
{
    public GameObject obj;
    public float dist;
    public Camera cam;
    public Color32 color;
    public List<Texture2D> SplashTexs;
    public int index = 0;
    public float shotHeight;
    // Start is called before the first frame update

    // Update is called once per frame
    void Update()
    {
        if (!cam) return;
        if (Input.GetKeyDown(KeyCode.Mouse0))
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
        }
    }
}
