using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MazeGenerator : MonoBehaviour
{
    private string[,] level;
    //private string[,] visited;

    private bool up = false;
    private bool down = false;
    private bool left = false;
    private bool right = false;

    private int r;


    // Start is called before the first frame update
    void Start()
    {
        int x = 6;
        level = new string[x, x];
        //visited = new string[x * x,2];

        int posX = 0;
        int posY = 0;

        for (int i = 0; i < (x*x); i++)
        {
            level[posX, posY] = "S";
            if (posX == 5)
            {
                posX = 0;
                posY++;
            }
            //if (posY == 5 && posX == 5)
            //{
               // break;
            //}

        }

        posX = Random.Range(0, 6);
        posY = Random.Range(0, 6);

        while (true)
        {
            up = false;
            down = false;
            left = false;
            right = false;

            level[posX, posY] = "P";

            if (posY != 0 && level[posX, posY - 1] == "S") { up = true; }
            if (posY != 5 && level[posX, posY + 1] == "S") { down = true; }
            if (posX != 0 && level[posX - 1, posY] == "S") { left = true; }
            if (posX != 0 && level[posX + 1, posY] == "S") { right = true; }

            if (up == false && down == false && left == false && right == false) { break; }

            r = Random.Range(0, 2);
            if (r != 1 && up == true) { level[posX, posY - 1] = "P"; }
            else if (up == true) {  level[posX, posY - 1] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && down == true) { level[posX, posY + 1] = "P"; }
            else if (down == true) { level[posX, posY + 1] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && left == true) { level[posX - 1, posY] = "P"; }
            else if (left == true) { level[posX - 1, posY] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && right == true) { level[posX + 1, posY - 1] = "P"; }
            else if (right == true) { level[posX + 1, posY - 1] = "#"; }

        }


    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
