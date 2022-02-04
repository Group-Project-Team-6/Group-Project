using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    private Rigidbody rigBody;
    public ParticleSystem ps;
    // Start is called before the first frame update
    void Start()
    {
        rigBody = transform.GetComponent<Rigidbody>();
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
        if (Input.GetKeyDown(KeyCode.Space))
        {
            rigBody.AddForce(transform.up * 1000, ForceMode.Impulse);
        }
    }

    void Update()
    {
        transform.Rotate(new Vector3(0, Input.GetAxis("Mouse X") * 10, 0));
    }

    void OnCollisionEnter(Collision collision)
    {
        Debug.Log( collision.contacts[0].point);
        ParticleSystem partSys = Instantiate<ParticleSystem>(ps,collision.contacts[0].point,new Quaternion());
        partSys.Play();
    }
}
