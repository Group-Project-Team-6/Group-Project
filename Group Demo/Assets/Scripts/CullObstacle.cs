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
        Shader.SetGlobalVector(
                   "_PlayerPos",
                   new Vector4(
                       player.transform.position.x,
                       player.transform.position.y,
                       player.transform.position.z,
                       1.0f));
        //RaycastHit hit;
        //Vector3 dist = (player.transform.position - transform.position);
        //hit = new RaycastHit();

        ////if something is raycasted besides player, tell shaders to cull the obstacles out
        //if (Physics.Raycast(transform.position, dist.normalized, out hit, Mathf.Infinity, 1))
        //{
        //    Debug.DrawLine(transform.position, hit.transform.position);
        //    if (hit.transform.gameObject != player)
        //    {

        //        //UnityEngine.Rendering.CompareFunction.
        //    }
        //    else
        //    {
        //        Shader.SetGlobalFloat("_IsCasted", 0.0f);
        //    }
        //}
    }
}
