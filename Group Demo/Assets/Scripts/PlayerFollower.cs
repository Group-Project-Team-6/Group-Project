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
        transform.LookAt(player.transform);
        float rot = Mathf.Min(transform.position.y - Input.GetAxis("Mouse Y") , player.transform.position.y + 2.0f);
        rot = Mathf.Max(rot, player.transform.position.y - 2.0f);
        transform.position = new Vector3(transform.position.x, rot , transform.position.z);
    }
}
