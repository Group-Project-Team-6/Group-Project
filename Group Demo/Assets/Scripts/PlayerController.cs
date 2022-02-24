using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseArgs : EventArgs
{
    public float rotateX;
    public float rotateY;
    public bool shoot;
}

public class PlayerController : MonoBehaviour
{
    public event EventHandler<MouseArgs> OnMouseEvent;
    private Rigidbody rigBody;
    public ParticleSystem ps;
    public bool fromServer;
    public float sensitivity;
    public bool fainted;
    private bool onGround;
    public bool useMouse;
    public KeyCode kforward;
    public KeyCode kback;
    public KeyCode kleft;
    public KeyCode kright;
    public KeyCode kjump;
    public KeyCode krLeft;
    public KeyCode krRight;
    public KeyCode krUp;
    public KeyCode krDown;
    public KeyCode kshoot;
    public float rotateX;
    public float rotateY;
    public bool shoot;

    // Start is called before the first frame update
    void Start()
    {
        rigBody = transform.GetComponent<Rigidbody>();
        onGround = true;
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;

    }

    // Update is called once per frame
    void FixedUpdate()
    {   if (fainted || fromServer)
        {
            return;
        }
        if (Input.GetKey(kforward))
        {
            rigBody.AddForce(transform.forward * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(kback))
        {
            rigBody.AddForce(-transform.forward * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(kright))
        {
            rigBody.AddForce(transform.right * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(kleft))
        {
            rigBody.AddForce(-transform.right * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(kjump) && onGround)
        {
            onGround = false;
            rigBody.AddForce(transform.up * 500, ForceMode.Impulse);
        }

    }

    void Update()
    {
        if (fainted || fromServer)
        {
            return;
        }
        MouseArgs args = new MouseArgs();
        if (useMouse)
        {
            rotateY = Input.GetAxis("Mouse X");
            rotateX = Input.GetAxis("Mouse Y");
            transform.Rotate(new Vector3(0, rotateY * sensitivity, 0));
            shoot = Input.GetMouseButton(0);
        }
        else
        {
            rotateY = 0.0f;
            rotateX = 0.0f;
            if (Input.GetKey(krLeft)) rotateY -= 10.0f * Time.deltaTime;
            if (Input.GetKey(krRight)) rotateY += 10.0f * Time.deltaTime;
            transform.Rotate(new Vector3(0, rotateY * sensitivity, 0));
            if (Input.GetKey(krUp)) rotateX += 10.0f * Time.deltaTime;
            if (Input.GetKey(krDown)) rotateX -= 10.0f * Time.deltaTime;
            shoot = Input.GetKey(kshoot);
        }
        args.shoot = shoot;
        args.rotateY = rotateY;
        args.rotateX = rotateX;
        OnMouseEvent(this,args);
    }

    void OnCollisionEnter(Collision collision)
    {
        if (fainted || fromServer)
        {
            return;
        }
        //Debug.Log( collision.contacts[0].point);
        if (Vector3.Dot(collision.contacts[0].normal, Vector3.up) > 0.5f)
        {
            onGround = true;
            return;
        }

        ParticleSystem partSys = Instantiate<ParticleSystem>(ps,collision.contacts[0].point,new Quaternion());
        partSys.Play();
        Destroy(partSys.gameObject, 2.0f);
    }
}
