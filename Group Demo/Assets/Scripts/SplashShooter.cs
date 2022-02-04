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
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Mouse0))
        {
            index++;
            if (index >= SplashTexs.Count) index = 0;
            GameObject g = Instantiate(obj, transform.position + transform.forward * dist, new Quaternion());
            if (g.GetComponent<Rigidbody>()) g.GetComponent<Rigidbody>().AddForce((transform.position - cam.transform.position).normalized * dist * 1000);
            if (g.GetComponent<SplashBall>()) { 
                SplashBall sb = g.GetComponent<SplashBall>();
                sb.color = this.color;
                sb.mass = 1;
                sb.radius = 1;
                sb.tex = SplashTexs[index];
            }
        }
    }
}
