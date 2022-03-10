using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Timer : MonoBehaviour
{
    public float timevalue = 90;

    // Update is called once per frame
    void Update()
    {
        timevalue -= Time.deltaTime;
    }
}
