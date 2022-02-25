using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimerText : MonoBehaviour
{
    TMPro.TextMeshProUGUI tmp;
    // Start is called before the first frame update
    void Start()
    {
        tmp = gameObject.GetComponent<TMPro.TextMeshProUGUI>();
        tmp.horizontalAlignment = TMPro.HorizontalAlignmentOptions.Center;
        tmp.verticalAlignment = TMPro.VerticalAlignmentOptions.Top;
    }

    // Update is called once per frame
    void Update()
    {
        tmp.text = "Time Left:\n" + GameManager.gameManager.gameTime.ToString("0") + "s";
    }
}
