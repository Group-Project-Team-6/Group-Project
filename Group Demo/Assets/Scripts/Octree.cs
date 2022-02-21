using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Octree : MonoBehaviour
{
    List<Octree> subtrees;
    List<GameObject> staticObjs;
    List<GameObject> dynamicObjs;
    int maxObj = 0;
    int maxLevel = 0;
    Vector3 pos = new Vector3(0, 0, 0);
    Vector3 dimension = new Vector3(0, 0, 0);
    bool init = false;


    public void Init(Vector3 pos, Vector3 dimension, int maxLevel, int maxObj)
    {
        this.pos = pos;
        this.dimension = dimension;
        this.maxLevel = maxLevel;
        this.maxObj = maxObj;
        dynamicObjs = null;
        staticObjs = null;
        subtrees = null;
        init = true;
    }

    void Insert(GameObject g, bool isStatic, int depthLeft, int maxobj)
    {
        if (!AABBTest(g.transform.position, pos, g.GetComponent<Renderer>().bounds.size, dimension)) return;
        if (subtrees != null)
        {
            for(int i = 0; i < 8; i++)
            {
                subtrees[i].Insert(g, isStatic, depthLeft - 1, maxobj);
            }
        }
        else
        {
            if (isStatic && staticObjs.Count < maxObj)
            {
                staticObjs.Add(g);
                return;
            }
            if (!isStatic && dynamicObjs.Count < maxObj)
            {
                dynamicObjs.Add(g);
                return;
            }

        }
        if (!init || depthLeft < 0) return;
        subtrees = new List<Octree>(8);
        int count = 0;
        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = -1; j <= 1; i += 2)
            {
                for (int k = -1; k <= 1; i += 2)
                {
                    Vector3 subpos = new Vector3(
                        dimension.x * 0.25f * i,
                        dimension.y * 0.25f * j,
                        dimension.z * 0.25f * k);
                    subtrees[count].Init(subpos,dimension * 0.5f, depthLeft - 1, maxobj);

                    subtrees[count].Insert(g, isStatic, depthLeft - 1,maxobj);
                }
            }
        }
    }

    void Track()
    {

    }

    bool AABBTest(Vector3 objPos, Vector3 treePos, Vector3 objSize, Vector3 treeDimen)
    {
        Vector3 delta = treePos - objPos;
        Vector3 totalSize = treeDimen + objSize;

        if (Mathf.Abs(delta.x) < totalSize.x && Mathf.Abs(delta.y) < totalSize.y && Mathf.Abs(delta.z) < totalSize.z)
        {
            return true;
        }
        return false;
    }
}
