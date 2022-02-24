using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioWalk : MonoBehaviour
{
    public FMODUnity.EventReference walkSound;
    FMOD.Studio.EventInstance walkState;
    bool playerismoving;
    public float walkingspeed;

    void Update()
    {
        if (Input.GetAxis("Vertical") >= 0.01f || Input.GetAxis("Horizontal") >= 0.01f || Input.GetAxis("Vertical") <= -0.01f || Input.GetAxis("Horizontal") <= -0.01f)
        {
            playerismoving = true;
        }
        else if (Input.GetAxis("Vertical") == 0 || Input.GetAxis("Horizontal") == 0)
        {
            playerismoving = false;
        }
    }

    void CallFootSteps()
    {
        if (playerismoving == true)
        {
            FMODUnity.RuntimeManager.PlayOneShot(walkSound);
        }
    }

    void Start()
    {
        InvokeRepeating ("CallFootSteps", 0, walkingspeed);

        walkState = FMODUnity.RuntimeManager.CreateInstance(walkSound);
        walkState.start();
    }

    void OnDisable()
    {
        playerismoving = false;
    }
}
