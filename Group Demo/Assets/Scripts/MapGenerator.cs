using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class MapGenerator : MonoBehaviour
{
    public MazeGenerator mazeGen;
    public ModuleMap mMap;
    public int unitLength = 10;
    public int width;
    public int length;
    List<string> maze;

    // Start is called before the first frame update
    void Start()
    { 
        mMap.LoadModuleMap();
        Generate();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Generate()
    {
        mazeGen.Generate(length, width);
        LoadMaze();
        for (int level = 0; level < maze.Count; level++)
        {
            for (int l = 0; l < length; l++)
            {
                for (int w = 0; w < width; w++)
                {
                    AddChild(GetSymbol(level, l, w),level,l,w);
                }
            }
        }
    }

    private char GetSymbol(int level, int l, int w)
    {
        //Debug.Log(level + " " + (l * width + w));
        return maze[level][l * width + w];
    }

    private void LoadMaze()
    {
        maze = new List<string>();
        int index = 0;
        while(index != -1)
        {
            try
            {
                string path = Path.Combine(Application.dataPath, "map" + index.ToString() + ".txt");
                StreamReader sr = new StreamReader(path);
                string level = "";
                string strLine = sr.ReadLine();
                while (strLine != null)
                {
                    level += strLine;
                    strLine = sr.ReadLine();
                }
                maze.Add(level);
                Debug.Log(maze[index]);
                sr.Close();
                index++;
            }
            catch (Exception e)
            {
                index = -1;
                Debug.LogError(e.Message);
            }
            finally
            {

            }
                
        }
       // Debug.Log("Leve: " + maze.Count);
    }

    private void AddChild(char symbol, int level, int l, int w)
    {
        if (symbol == 'S' || !mMap.moduleMap.ContainsKey(symbol)) return;
        GameObject g = Instantiate(
            mMap.bank[mMap.moduleMap[symbol]],
            new Vector3(l * unitLength, level * unitLength, w * unitLength),
            new Quaternion(), transform);
        g.transform.localScale = new Vector3(unitLength, unitLength, unitLength);
    }

    private void DeleteChild()
    {
        for(int i = transform.childCount - 1; i >= 0; i--)
        {
            Destroy(transform.GetChild(i).gameObject);
        }
    }
}
