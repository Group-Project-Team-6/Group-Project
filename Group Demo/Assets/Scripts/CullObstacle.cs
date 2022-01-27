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
        /*
        while (hit.transform == null || hit.transform.gameObject != player)
        {
            if (Physics.Raycast(transform.position, dist.normalized, out hit, Mathf.Infinity, 1))
            {
                if (hit.transform.gameObject != player)
                {
                    Debug.Log(hit.transform.gameObject.name);
                    Debug.Log(hit.transform.gameObject.layer);
                    hideList.Add(hit.transform.gameObject);
                    lastFrame.Remove(hit.transform.gameObject);
                    Debug.Log(lastFrame.Count);
                    Debug.Log(hideList.Count);
                    hit.transform.gameObject.layer = 6;
                }
            }
        }
        
        for (int i = 0; i < hideList.Count; i++)
        {
            Color c = hideList[i].GetComponent<Renderer>().material.color;
            //Vector3 v = player.transform.position;
            hideList[i].GetComponent<Renderer>().material.SetFloat("_IsCasted",1.0f);// = true;
            hideList[i].layer = 0;
            Debug.Log(hideList[i].layer);
        }
        for(int j = 0; j < lastFrame.Count; j++)
        {
            Color c = lastFrame[j].GetComponent<Renderer>().material.color;
            //Vector3 v = player.transform.position;
            lastFrame[j].GetComponent<Renderer>().material.SetFloat("_IsCasted",0.0f);

            //lastFrame[j].GetComponent<Renderer>().forceRenderingOff = false;
            lastFrame[j].layer = 0;
        }
        
        lastFrame = new List<GameObject>(hideList);
        hideList.Clear();
        */
    }
}
