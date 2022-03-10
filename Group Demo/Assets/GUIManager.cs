using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
public class GUIManager : MonoBehaviour
{
    public TextMeshProUGUI mText;
    // Start is called before the first frame update
    void Awake()
    {

    }
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        mText.text = "Time remaining: " + 10;
    }
}
