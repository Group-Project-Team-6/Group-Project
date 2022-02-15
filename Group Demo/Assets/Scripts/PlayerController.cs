using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    private Rigidbody rigBody;
    public ParticleSystem ps;
    public float sensitivity;
    private bool onGround;
    // Start is called before the first frame update
    void Start()
    {
        rigBody = transform.GetComponent<Rigidbody>();
        onGround = true;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (Input.GetKey(KeyCode.W))
        {

            rigBody.AddForce(transform.forward * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(KeyCode.S))
        {
            rigBody.AddForce(-transform.forward * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(KeyCode.D))
        {
            rigBody.AddForce(transform.right * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(KeyCode.A))
        {
            rigBody.AddForce(-transform.right * 50, ForceMode.Impulse);
        }
        if (Input.GetKey(KeyCode.Space) && onGround)
        {
            onGround = false;
            rigBody.AddForce(transform.up * 500, ForceMode.Impulse);
        }
    }

    void Update()
    {
        transform.Rotate(new Vector3(0, Input.GetAxis("Mouse X") * sensitivity, 0));
    }

    void OnCollisionEnter(Collision collision)
    {
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
