using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MazeGenerator : MonoBehaviour
{
    private string[,] level;

    private int[,] potentialNodes;
    int numberOfNodes;
    private int[] usedNodes;

    private bool up = false;
    private bool down = false;
    private bool left = false;
    private bool right = false;

    private int r;


    // Start is called before the first frame update
    void Start()
    {
        numberOfNodes = 0;

        int x = 6;
        level = new string[x, x];
        //visited = new string[x * x,2];

        int posX = 0;
        int posY = 0;

        for (int i = 0; i < (x * x); i++)
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

            if (up == false && down == false && left == false && right == false) { break; } // add


            if (up == true) { Assign(posX, posY - 1); }
            if (down == true) { Assign(posX, posY + 1); }
            if (left == true) { Assign(posX - 1, posY); }
            if (right == true) { Assign(posX + 1, posY); }


            /*
            r = Random.Range(0, 2);
            if (r != 1 && up == true) { level[posX, posY - 1] = "P"; }
            else if (up == true) { level[posX, posY - 1] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && down == true) { level[posX, posY + 1] = "P"; }
            else if (down == true) { level[posX, posY + 1] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && left == true) { level[posX - 1, posY] = "P"; }
            else if (left == true) { level[posX - 1, posY] = "#"; }

            r = Random.Range(0, 2);
            if (r != 1 && right == true) { level[posX + 1, posY - 1] = "P"; }
            else if (right == true) { level[posX + 1, posY - 1] = "#"; }
            */

            while (true)
            {
                int next = Random.Range(0, numberOfNodes - 1);
                posX = potentialNodes[next, 0];
                posY = potentialNodes[next, 1];

                bool check = false;
                for (int i = 0; i < numberOfNodes; i++)
                {
                    if (usedNodes[i] == next) { check = true; }
                }
                if (check == false) {

                    usedNodes[usedNodes.Length + 1] = next;

                    break;
                }
            }




        }


    }

    void Assign(int x, int y)
    {
        r = Random.Range(0, 2);
        if (r != 1) {
            level[x, y] = "P";
            potentialNodes[numberOfNodes, 0] = x;
            potentialNodes[numberOfNodes, 1] = y;
            numberOfNodes++;
        }
        else { level[x, y] = "#"; }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
