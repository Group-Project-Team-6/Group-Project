//Project: CSC8507 Group Project Unity Prototype
//Author: Chris Hui

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CullObstacle : MonoBehaviour
{
    public GameObject player;
    private List<GameObject> hideList, lastFrame;
    // Start is called before the first frame update
    void Start()
    {
        hideList = new List<GameObject>();
        lastFrame = new List<GameObject>();
    }

    // Update is called once per frame
    void Update()
    {   
        RaycastHit hit;
        Vector3 dist = (player.transform.position - transform.position);
        hit = new RaycastHit();

        //if something is raycasted besides player, tell shaders to cull the obstacles out
        if (Physics.Raycast(transform.position, dist.normalized, out hit, Mathf.Infinity, 1))
        {
            if (hit.transform.gameObject != player)
            {
                Shader.SetGlobalFloat("_IsCasted", 1.0f);
            }
            else
            {
                Shader.SetGlobalFloat("_IsCasted", 0.0f);
            }
        }
    }
}
