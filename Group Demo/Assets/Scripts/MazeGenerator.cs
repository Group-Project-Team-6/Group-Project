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
        level = new string[length, height];

        numberOfStairs = 0;
        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map.txt");

        MazeGenCheck();
        numberOfStairs = 0;
        AddStairs();
        PathToStairs();
        LevelTextFile("/map1.txt");

        MazeGenCheck();
        PathToStairs();
        LevelTextFile("/map2.txt");

    }

    // Update is called once per frame
    void Update()
    {

    }

    public void Generate(int l, int w) 
    {
        length = w;
        height = l;

        level = new string[length, height];

        numberOfStairs = 0;
        MazeGenCheck();
        AddStairs();
        LevelTextFile("/map.txt");

        MazeGenCheck();
        numberOfStairs = 0;
        AddStairs();
        PathToStairs();
        LevelTextFile("/map1.txt"); // layered maps

        MazeGenCheck();
        PathToStairs();
        LevelTextFile("/map2.txt");
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

                if (level[tempX, tempY] == "P" && neighbourStairCheck())
                {
                    if (tempY != 0) { if (level[tempX, tempY - 1] == "P") { up = true; optionsCount++; } }
                    if (tempY != (height - 1)) { if (level[tempX, tempY + 1] == "P") { down = true; optionsCount++; } }
                    if (tempX != 0) { if (level[tempX - 1, tempY] == "P") { left = true; optionsCount++; } }
                    if (tempX != (length - 1)) { if (level[tempX + 1, tempY] == "P") { right = true; optionsCount++; } }

                    if (optionsCount == 1)
                    {
                        if (up == true) { level[tempX, tempY] = "A"; numberOfStairs++; }
                        if (down == true) { level[tempX, tempY] = "V"; numberOfStairs++; }
                        if (left == true) { level[tempX, tempY] = "<"; numberOfStairs++; }
                        if (right == true) { level[tempX, tempY] = ">"; numberOfStairs++; }
                    }

                    else if (optionsCount == 2)
                    {
                        if ((up == true && down == true) || (left == true && right == true)) { }
                        else
                        {
                            if (up == true && left == true && level[tempX - 1, tempY - 1] == "P") { level[tempX, tempY] = "A"; numberOfStairs++; }
                            if (up == true && right == true && level[tempX + 1, tempY - 1] == "P") { level[tempX, tempY] = ">"; numberOfStairs++; }
                            if (down == true && left == true && level[tempX - 1, tempY + 1] == "P") { level[tempX, tempY] = "<"; numberOfStairs++; }
                            if (down == true && right == true && level[tempX + 1, tempY + 1] == "P") { level[tempX, tempY] = "V"; numberOfStairs++; }

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

            if (numberOfStairs >= ((length * height) / 80)) { StairPositions(); break; }
        }
    }

    bool neighbourStairCheck()
    {
        bool check = true;
        if (tempY != 0) { if (level[tempX, tempY - 1] == "A" || level[tempX, tempY - 1] == "V" || level[tempX, tempY - 1] == "<" || level[tempX, tempY - 1] == ">") { check = false; } }
        else if (tempY != (height - 1)) { if (level[tempX, tempY + 1] == "A" || level[tempX, tempY + 1] == "V" || level[tempX, tempY + 1] == "<" || level[tempX, tempY + 1] == ">") { check = false; } }
        else if (tempX != 0) { if (level[tempX - 1, tempY] == "<" || level[tempX - 1, tempY] == ">" || level[tempX - 1, tempY] == "A" || level[tempX - 1, tempY] == "V") { check = false; } }
        else if (tempX != (length - 1)) { if (level[tempX + 1, tempY] == "<" || level[tempX + 1, tempY] == ">" || level[tempX + 1, tempY] == "A" || level[tempX + 1, tempY] == "V") { check = false; } }
        return check;
    }

    void StairPositions()
    {
        stairPos = new int[numberOfStairs + 1, 2];

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

            bool stairCheck = true;
            pathCounter = 0;
            for (int i = 0; i < height; i++) // check the number of paths
            {
                for (int z = 0; z < length; z++)
                {
                    if (level[z, i] == "P") { pathCounter++; }
                }
            }
            if (pathCounter >= ((length * height) / 2) && stairCheck) { break; }
        }
    }

    void PathToStairs() 
    {
        bool stairCheck = false;

        for (int i = 0; i < height; i++)
        {
            for (int z = 0; z < length; z++)
            {
                if (level[z, i] == "S")
                {
                    stairCheck = false;
                    if (i != 0) { if (level[z, i - 1] == "P") { stairCheck = true; } }
                    if (i != (height - 1)) { if (level[z, i + 1] == "P") { stairCheck = true; } }
                    if (z != 0) { if (level[z - 1, i] == "P") { stairCheck = true; } }
                    if (z != (length - 1)) { if (level[z + 1, i] == "P") { stairCheck = true; } }


                    if (stairCheck == false) // Create a path to stairs
                    {
                        int x = z;
                        int y = i;
                        while (stairCheck == false)
                        {
                            up = false;
                            down = false;
                            left = false;
                            right = false;

                            if (y != 0) { if (level[x, y - 1] == "#") { up = true; } }
                            if (y != (height - 1)) { if (level[x, y + 1] == "#") { down = true; } }
                            if (x != 0) { if (level[x - 1, y] == "#") { left = true; optionsCount++; } }
                            if (x != (length - 1)) { if (level[x + 1, y] == "#") { right = true; } }


                            int nextDirection = Random.Range(0, 4);

                            if (nextDirection == 0)
                            {
                                if (up == true) { level[x, y - 1] = "T"; y--; }
                                else { nextDirection++; }
                            }
                            if (nextDirection == 1)
                            {
                                if (down == true) { level[x, y + 1] = "T"; y++; }
                                else { nextDirection++; }
                            }
                            if (nextDirection == 2)
                            {
                                if (left == true) { level[x - 1, y] = "T"; x--; }
                                else { nextDirection++; }
                            }
                            if (nextDirection == 3)
                            {
                                if (right == true) { level[x + 1, y] = "T"; x++; }
                                else { nextDirection++; }
                            }



                            Debug.Log("Y = " + y);

                            if (y != 0) { if (level[x, y - 1] == "P") { stairCheck = true; break; } }
                            if (y != (height - 1)) { if (level[x, y + 1] == "P") { stairCheck = true; break; } }
                            if (x != 0) { if (level[x - 1, y] == "P") { stairCheck = true; break; } }
                            if (x != (length - 1)) { if (level[x + 1, y] == "P") { stairCheck = true; break; } }

                        }
                    }
                }
            }
        }

        for (int i = 0; i < height; i++)
        {
            for (int z = 0; z < length; z++) 
            { 
                if (level[z, i] == "T") { level[z, i] = "P"; }
            }
        }
        
    }

    void MazeGen()
    {

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
                        if (tempY != 0) { if (level[tempX, tempY - 1] == "S") { posX = tempX; posY = tempY; break; } }
                        if (tempY != (height - 1)) { if (level[tempX, tempY + 1] == "S") { posX = tempX; posY = tempY; break; } }
                        if (tempX != 0) { if (level[tempX - 1, tempY] == "S") { posX = tempX; posY = tempY; break; } }
                        if (tempX != (length - 1)) { if (level[tempX + 1, tempY] == "S") { posX = tempX; posY = tempY; break; } }

                    }

                    //Debug.Log("tempX = " + tempX);
                    //Debug.Log("tempY = " + tempY);

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
