//Project: CSC8507 Group Project Unity Prototype
//Author: Chris Hui

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerFollower : MonoBehaviour
{
    public GameObject player;
    CapsuleCollider cCol;
    // Start is called before the first frame update
    void Start()
    {
        cCol = player.transform.GetComponent<CapsuleCollider>();
    }

    // Update is called once per frame
    void Update()
    { 
        float rot = Mathf.Min(transform.position.y - Input.GetAxis("Mouse Y") , player.transform.position.y + 3.0f);
        rot = Mathf.Max(rot, player.transform.position.y - (cCol.height / 3.0f));

        transform.position = new Vector3(transform.position.x, rot , transform.position.z);
        transform.LookAt(player.transform.position + player.transform.up * 2);
    }
}
