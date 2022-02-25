using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPDisplay : MonoBehaviour
{
    public int playerNum;
    RectTransform t;

    // Start is called before the first frame update
    void Start()
    {
        t = transform.GetComponent<RectTransform>();
    }

    // Update is called once per frame
    void Update()
    {
        Player p = GameManager.gameManager.players[playerNum - 1].GetComponent<Player>();
        if (p)
        {
            t.sizeDelta = new Vector2(400.0f * (float)(p.Health) / (float)(p.maxHealth), t.sizeDelta.y);
        }
    }
}
