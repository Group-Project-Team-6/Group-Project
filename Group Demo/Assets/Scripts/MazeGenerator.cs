using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class MazeGenerator : MonoBehaviour
{

    private string[,] level;

    private bool up = false;
    private bool down = false;
    private bool left = false;
    private bool right = false;

    private int posX;
    private int posY;

    private int tempX;
    private int tempY;

    public int length;
    public int height;

    bool end;

    private int randInt1;
    private bool check;
    private int optionsCount;

    private int numberOfStairs;
    private int[,] stairPos;


    // Start is called before the first frame update
    void Start()
    {
        numberOfStairs = 0;
        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map.txt");

        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map2.txt");

        MazeGenCheck();
        LevelTextFile("/map3.txt");

    }

    // Update is called once per frame
    void Update()
    {

    }

    void Generate(int l, int w) 
    {
        length = w;
        height = l;

        numberOfStairs = 0;
        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map.txt");

        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map2.txt"); // layered maps

        MazeGenCheck();
        LevelTextFile("/map3.txt");
    }

    void LevelTextFile(string fileName)
    {
        string mapline = ""; // create text file
        string map = Application.dataPath + fileName;
        File.WriteAllText(map, "");

        for (int i = 0; i < height; i++)
        {
            for (int z = 0; z < length; z++)
            {
                mapline = mapline + level[z, i];
            }

            File.AppendAllText(map, mapline + "\n");

            mapline = null;
        }
    }

    void AddStairs()
    {
        tempX = 0;
        tempY = 0;
        int numStairs = 0;
        while (true)
        {
            up = false;
            down = false;
            left = false;
            right = false;
            optionsCount = 0;

            int r = Random.Range(0,3);
            if (r == 1)
            {

                if (level[tempX, tempY] == "P")
                {
                    if (tempY != 0) { if (level[tempX, tempY - 1] == "P") { up = true; optionsCount++; } }
                    if (tempY != (height - 1)) { if (level[tempX, tempY + 1] == "P") { down = true; optionsCount++; } }
                    if (tempX != 0) { if (level[tempX - 1, tempY] == "P") { left = true; optionsCount++; } }
                    if (tempX != (length - 1)) { if (level[tempX + 1, tempY] == "P") { right = true; optionsCount++; } }

                    if (optionsCount == 1)
                    {
                        if (up == true) { level[tempX, tempY] = "A"; numStairs++; }
                        if (down == true) { level[tempX, tempY] = "V"; numStairs++; }
                        if (left == true) { level[tempX, tempY] = "<"; numStairs++; }
                        if (right == true) { level[tempX, tempY] = ">"; numStairs++; }
                    }

                    else if (optionsCount == 2)
                    {
                        if ((up == true && down == true) || (left == true && right == true)) { }
                        else
                        {
                            if (up == true && left == true && level[tempX - 1, tempY - 1] == "P") { level[tempX, tempY] = "A"; numStairs++; }
                            if (up == true && right == true && level[tempX + 1, tempY - 1] == "P") { level[tempX, tempY] = ">"; numStairs++; }
                            if (down == true && left == true && level[tempX - 1, tempY + 1] == "P") { level[tempX, tempY] = "<"; numStairs++; }
                            if (down == true && right == true && level[tempX + 1, tempY + 1] == "P") { level[tempX, tempY] = "V"; numStairs++; }

                        }
                    }


                }
            }

            tempX++;
            if (tempX == length)
            {
                tempX = 0;
                tempY++;
            }
            if (tempY == height) { AddStairs(); }

            if (numStairs >= 3) { numberOfStairs = numStairs; StairPositions(); break; }
        }
    }

    void StairPositions()
    {
        stairPos = new int[numberOfStairs, 2];

        int x = 0;
        for (int i = 0; i < height; i++)
        {
            for (int z = 0; z < length; z++)
            {
                if (level[z, i] == "A" || level[z, i] == "V" || level[z, i] == "<" || level[z, i] == ">") 
                {
                    stairPos[x, 0] = z;
                    stairPos[x, 1] = i;
                    x++;
                }
            }
        }

    }

    void MazeGenCheck()
    {
        int pathCounter;
        while (true)
        {
            MazeGen();

            pathCounter = 0;
            for (int i = 0; i < height; i++) // check the number of paths
            {
                for (int z = 0; z < length; z++)
                {
                    if (level[z, i] == "P") { pathCounter++; }
                }
            }
            if (pathCounter >= ((length * height) / 2)) { break; }
        }
    }

    void MazeGen()
    {
        level = new string[length, height];


        for (int i = 0; i < height; i++) // populate
        {
            for (int z = 0; z < length; z++)
            {
                level[z, i] = "S";
            }
        }

        posX = Random.Range(0, length); // pick random start
        posY = Random.Range(0, height);
        level[posX, posY] = "P";

        end = false;
        while (end == false)
        {

            up = false;
            down = false;
            left = false;
            right = false;
            optionsCount = 0;


            if (posY != 0) { if (level[posX, posY - 1] == "S") { up = true; optionsCount++; } } // check if empty
            if (posY != (height - 1)) { if (level[posX, posY + 1] == "S") { down = true; optionsCount++; } }
            if (posX != 0) { if (level[posX - 1, posY] == "S") { left = true; optionsCount++; } }
            if (posX != (length - 1)) { if (level[posX + 1, posY] == "S") { right = true; optionsCount++; } }


            if (up == false && down == false && left == false && right == false)
            {
                tempX = 0;
                tempY = 0;
                while (true) // find new start
                {
                    if (level[tempX, tempY] == "P")
                    {
                        if (posY != 0) { if (level[posX, posY - 1] == "S") { posX = tempX; posY = tempY; break; } }
                        else if (posY != (height - 1)) { if (level[posX, posY + 1] == "S") { posX = tempX; posY = tempY; break; } }
                        else if (posX != 0) { if (level[posX - 1, posY] == "S") { posX = tempX; posY = tempY; break; } }
                        else if (posX != (length - 1)) { if (level[posX + 1, posY] == "S") { posX = tempX; posY = tempY; break; } }

                    }

                    tempX++;
                    if (tempX == length)
                    {
                        tempX = 0;
                        tempY++;
                    }
                    if (tempY == height) { end = true; break; }
                }

            }

            else
            {
                randInt1 = 4;
                if (optionsCount > 1)
                {
                    check = true;
                    while (check)
                    {
                        randInt1 = Random.Range(0, 4); // pick next wall
                        switch (randInt1)
                        {
                            case 0:
                                if (up == true)
                                {
                                    check = false;
                                    level[posX, posY - 1] = "#";
                                }
                                break;
                            case 1:
                                if (down == true)
                                {
                                    check = false;
                                    level[posX, posY + 1] = "#";
                                }
                                break;
                            case 2:
                                if (left == true)
                                {
                                    check = false;
                                    level[posX - 1, posY] = "#";
                                }
                                break;
                            case 3:
                                if (right == true)
                                {
                                    check = false;
                                    level[posX + 1, posY] = "#";
                                }
                                break;
                        }

                    }
                }
                check = true;
                while (check) // pick next path
                {
                    int randInt2;

                    if (randInt1 != 4)
                    {
                        while (true) // check to not replace wall with path
                        {
                            randInt2 = Random.Range(0, 4);
                            if (randInt2 != randInt1) { break; }
                        }
                    }
                    else
                    {
                        randInt2 = Random.Range(0, 4);
                    }
                    switch (randInt2)
                    {
                        case 0:
                            if (up == true)
                            {
                                check = false;
                                level[posX, posY - 1] = "P";
                                posY = posY - 1;
                            }
                            break;
                        case 1:
                            if (down == true)
                            {
                                check = false;
                                level[posX, posY + 1] = "P";
                                posY = posY + 1;
                            }
                            break;
                        case 2:
                            if (left == true)
                            {
                                check = false;
                                level[posX - 1, posY] = "P";
                                posX = posX - 1;
                            }
                            break;
                        case 3:
                            if (right == true)
                            {
                                check = false;
                                level[posX + 1, posY] = "P";
                                posX = posX + 1;
                            }
                            break;
                    }

                }
            }
        }

        for (int i = 0; i < height; i++) // fill gaps
        {
            for (int z = 0; z < length; z++)
            {
                if (level[z, i] == "S") { level[z, i] = "#"; }
            }
        }

        
        int paths;
        for (int i = 0; i < height; i++) // add connecting paths
        {
            for (int z = 0; z < length; z++)
            {
                if (level[z, i] == "#") 
                {
                    paths = 0;
                    if (i != 0) { if (level[z, i - 1] == "P") { paths++; } }
                    if (i != (height - 1)) { if (level[z, i + 1] == "P") { paths++; } }
                    if (z != 0) { if (level[z - 1, i] == "P") { paths++; } }
                    if (z != (length - 1)) { if (level[z + 1, i] == "P") { paths++; } }

                    if (paths > 1) {
                        int r = Random.Range(0, 3);
                        if (r == 0)
                        {
                            level[z, i] = "P";
                        }
                    }
                }
            }
        }

        if (numberOfStairs > 0) {
            for (int i = 0; i < height; i++) // create space for stairs
            {
                for (int z = 0; z < length; z++)
                {
                    for (int x = 0; x < numberOfStairs; x++) 
                    { 
                        if (z == stairPos[x, 0] && i == stairPos[x, 1]) { level[z, i] = "S"; }
                    }
                }
            }
        }
    }

}
