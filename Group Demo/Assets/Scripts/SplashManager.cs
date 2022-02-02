using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplashManager : MonoBehaviour
{
    public int Approach = 1;
    public bool ShowDebug = false;
    public GameObject splashRadius;
    public static int approach = 1;
    public static List<Splash> splashes;
    public static SplashManager instance;

    public static List<GameObject> debugs;
    public static GameObject splashR;
    // Start is called before the first frame update
    void Awake()
    {
        if(instance != null)
        {
            Destroy(gameObject);
        }
        else
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
            debugs = new List<GameObject>();
            splashes = new List<Splash>();
        }
    }

    void Update()
    {
        splashR = splashRadius;
        approach = Approach;
        ClearDebug();
        if (ShowDebug)
        {
            Debug();
        }
    }

    public static void Debug()
    {
        for(int i = 0; i < splashes.Count; i++)
        {
            GameObject g = Instantiate(splashR);
            g.transform.position = splashes[i].position;
            debugs.Add(g);
        }
    }

    public static void ClearDebug()
    {
        for (int i = 0; i < debugs.Count; i++)
        {
            Destroy(debugs[i]);
        }
        debugs.Clear();
    }
}
