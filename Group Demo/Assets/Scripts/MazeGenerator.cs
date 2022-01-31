using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MazeGenerator
{
    public List<string> maze;
    public int width;
    public int length;
    // Start is called before the first frame update

    public void Generate(int length, int width)
    {
        this.width = width;
        this.length = length;
        maze.Clear();
        for (int lv = 0; lv < 3; lv++) {
            Random.InitState(lv);
            string roll = "";
            for (int l = 0; l < length; l++)
            {
                for (int w = 0; w < width; w++)
                {
                    float r = Random.Range(0.0f, 1.0f);
                    if(lv > 0 && maze[lv-1][l * width + w] == 'V')
                    {
                        roll += "S";
                    }
                    else if (r < 0.6)
                    {
                        roll += "P";
                    } else if (r < 0.65)
                    {
                        roll += "V";
                    }
                    else
                    {
                        roll += "#";
                    }
                }
            }
            maze.Add(roll);
        }
    }
}
