using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPText : MonoBehaviour
{
    public int playerNum;
    TMPro.TextMeshProUGUI tmp;
    // Start is called before the first frame update
    void Start()
    {
        tmp = gameObject.GetComponent<TMPro.TextMeshProUGUI>();

    }

    // Update is called once per frame
    void Update()
    {
        Player p = GameManager.gameManager.players[playerNum - 1].GetComponent<Player>();
        if (p)
        {
            tmp.text = p.Health.ToString();
        }
    }
}
