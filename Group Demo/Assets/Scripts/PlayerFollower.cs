//Project: CSC8507 Group Project Unity Prototype
//Author: Chris Hui

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerFollower : MonoBehaviour
{
    public GameObject player;
    public float height;
    public float sensitivity;
    CapsuleCollider cCol;
    float angle;
    // Start is called before the first frame update
    void Start()
    {
        cCol = player.transform.GetComponent<CapsuleCollider>();
        angle = 0;
    }

    // Update is called once per frame
    void Update()
    {
        angle -= Input.GetAxis("Mouse Y") * sensitivity;
        //float rot = Mathf.Min(transform.position.y - Input.GetAxis("Mouse Y") , player.transform.position.y + 3.0f);
        //rot = Mathf.Max(rot, player.transform.position.y - (cCol.height / 3.0f));
        angle = Mathf.Clamp(angle, -70, 45);
        transform.localPosition = new Vector3(transform.localPosition.x, 10* Mathf.Sin(Mathf.Deg2Rad*angle) + height, -5 * Mathf.Cos(Mathf.Deg2Rad*angle));
        transform.LookAt(player.transform.position + new Vector3(0,height * 0.5f,0));
    }
}
