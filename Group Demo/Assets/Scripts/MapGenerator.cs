using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapGenerator : MonoBehaviour
{
    MazeGenerator mazeGen;
    public Dictionary<char,GameObject> moduleMap;
    public List<GameObject> bank;
    public int unitLength = 10;
    public int width;
    public int length;

    // Start is called before the first frame update
    void Start()
    {
        moduleMap = new Dictionary<char, GameObject>();
        moduleMap.Add('#', bank[0]);
        moduleMap.Add('P', bank[1]);
        moduleMap.Add('V', bank[2]);
        mazeGen = new MazeGenerator(); ;
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
        if (symbol == 'S') return;
        GameObject g = Instantiate(moduleMap[symbol], new Vector3(l * unitLength, level * unitLength, w * unitLength), new Quaternion(), transform);
        g.transform.localScale = new Vector3(unitLength, unitLength,unitLength);
    }

    private void DeleteChild()
    {
        for(int i = transform.childCount - 1; i >= 0; i--)
        {
            Destroy(transform.GetChild(i).gameObject);
        }
    }

}
