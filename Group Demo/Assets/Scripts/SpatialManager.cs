using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class SpatialManager : MonoBehaviour
{
    public abstract void AddObject(GameObject g, bool isStatic);

    public abstract void RemoveObject(GameObject g);

    public abstract void ChangeMode(GameObject g, bool isStatic);
}
