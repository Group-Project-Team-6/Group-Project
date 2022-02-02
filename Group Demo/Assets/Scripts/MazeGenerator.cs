using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

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

    private bool end = false;

    private int r;


    // Start is called before the first frame update
    void Start()
    {
        int x = 6;
        numberOfNodes = 0;
        potentialNodes = new int[(x*x),2];
        usedNodes = new int[(x*x)];
        level = new string[x, x];

        int posX = 0;
        int posY = 0;

        //for (int i = 0; i < ((x * x) - 1); i++)
        while (true)
        {
            level[posX, posY] = "S";
            posX++;
            if (posX == x)
            {
                posX = 0;
                posY++;
            }
            if (posY == x) { break; }

        }

        posX = Random.Range(0, 6);
        posY = Random.Range(0, 6);

        //level[posX, posY] = "P";

        while (true)
        {
            up = false;
            down = false;
            left = false;
            right = false;

            level[posX, posY] = "P";

            if (posY != 0 && level[posX, posY - 1] == "S") { up = true; } // Check neighbours 
            if (posY != 5 && level[posX, posY + 1] == "S") { down = true; }
            if (posX != 0 && level[posX - 1, posY] == "S") { left = true; }
            if (posX != 5 && level[posX + 1, posY] == "S") { right = true; }

            if (up == true || down == true || left == true || right == true)
            {

                if (up == true) { Assign(posX, posY - 1, ref potentialNodes, ref numberOfNodes); } // Assign values to neighbours and store potential new points
                if (down == true) { Assign(posX, posY + 1, ref potentialNodes, ref numberOfNodes); }
                if (left == true) { Assign(posX - 1, posY, ref potentialNodes, ref numberOfNodes); }
                if (right == true) { Assign(posX + 1, posY, ref potentialNodes, ref numberOfNodes); }

                while (true) // Pick a new point
                {

                    if (numberOfNodes == 1) 
                    {
                        int i = 0;
                        int z = 0;
                        while (true) // infinite loop issue
                        {
                            if (level[i, z] == "S")
                            {
                                posX = i;
                                posY = z;
                                break;
                            }

                            if (i == 5 && z == 5)
                            {
                                end = true;
                                break;
                            }

                            i++;
                            if (i == 6)
                            {
                                i = 0;
                                z++;
                            }

                        }

                        break; 
                    }

                    int next = Random.Range(0, numberOfNodes);
                    posX = potentialNodes[next, 0];
                    posY = potentialNodes[next, 1];

                    bool check = false;
                    for (int i = 0; i < numberOfNodes; i++)
                    {
                        if (usedNodes[i] == next) { check = true; }
                    }
                    if (check == false)
                    {

                        Debug.Log(numberOfNodes + "---");
                        usedNodes[usedNodes.Length - 1] = next;

                        break;
                    }

                    /*
                    if (j == 2000) 
                    {
                        Debug.Log(numberOfNodes + " ending ---");
                        end = true;
                        break; 
                    }
                    */
                }
            }
            /*
            else
            {
                int i = 0;
                int z = 0;
                while (true)
                {
                    if (level[i,z] == "S")
                    {
                        posX = i;
                        posY = z;
                        break;
                    }

                    if (i == 4 && z == 4)
                    {
                        end = true;
                        break;
                    }

                    i++;
                    if (i == 6)
                    {
                        i = 0;
                        z++;
                    }
                    
                }
            }
            */
            //if (up == false && down == false && left == false && right == false) { end = true; }

            if (end == true) { break; }

        }

        
        string mapline = "";
        string map = Application.dataPath + "/map.txt";
        File.WriteAllText(map, "");


        for (int i = 0; i < x; i++)
        {
            for (int z = 0; z < x; z++)
            {
                mapline = mapline + level[z, i];
            }

            File.AppendAllText(map, mapline + "\n");

            mapline = null;
        }
        

    }

    void Assign(int x, int y, ref int[,] pN, ref int nON)
    {
        r = Random.Range(0, 2);
        if (r != 1) {
            level[x, y] = "P";
            pN[nON, 0] = x;
            pN[nON, 1] = y;
            nON++;
        }
        else { level[x, y] = "#"; }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
