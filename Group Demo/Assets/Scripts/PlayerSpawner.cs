using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerSpawner : MonoBehaviour
{
    public List<GameObject> player;
    public GameObject playerManager;
    // Start is called before the first frame update
    public GameObject Spawn(int i, Vector3 pos, Transform t)
    {
        if (i >= player.Count || i < 0) return null;
        GameObject p = Instantiate(player[i], pos, new Quaternion(), t);
        p.layer = 10 + i;
        return p;
    }
}
