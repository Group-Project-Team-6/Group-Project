using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreAnim : MonoBehaviour
{
    public int speed = 10;

    void Update()
    {
        transform.Rotate(speed * Time.deltaTime, speed * Time.deltaTime, speed * Time.deltaTime);
    }
}
