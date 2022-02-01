using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapGenerator : MonoBehaviour
{
    MazeGenerator mazeGen;
    public ModuleMap mMap;
    public int unitLength = 10;
    public int width;
    public int length;

    // Start is called before the first frame update
    void Start()
    { 
        mazeGen = new MazeGenerator();
        mMap.LoadModuleMap();
        Generate();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Generate()
    {
        mazeGen.maze = new List<string>();
        mazeGen.Generate(length, width);
        DeleteChild();
        for (int level = 0; level < mazeGen.maze.Count; level++)
        {
            for (int l = 0; l < mazeGen.length; l++)
            {
                for (int w = 0; w < mazeGen.width; w++)
                {
                    AddChild(GetSymbol(level, l, w),level,l,w);
                }
            }
        }
    }

    private char GetSymbol(int level, int l, int w)
    {
        return mazeGen.maze[level][l* mazeGen.width + w];
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
