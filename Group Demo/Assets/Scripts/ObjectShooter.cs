using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectShooter : MonoBehaviour
{
    public GameObject obj;
    public float dist;
    public Camera cam;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Mouse0))
        {
            GameObject g = Instantiate(obj,transform.position + transform.forward * dist, new Quaternion());
            if (g.GetComponent<Rigidbody>()) g.GetComponent<Rigidbody>().AddForce((transform.position - cam.transform.position).normalized * dist * 1000);
        }
    }
}