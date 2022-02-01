//Project: CSC8507 Group Project Unity Prototype
//Author: Chris Hui

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerFollower : MonoBehaviour
{
    public GameObject player;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float rot = Mathf.Min(transform.position.y - Input.GetAxis("Mouse Y") , player.transform.position.y + 2.0f);
        rot = Mathf.Max(rot, player.transform.position.y - 1.0f);
        transform.position = new Vector3(transform.position.x, rot , transform.position.z);
        transform.LookAt(player.transform);
    }
}
