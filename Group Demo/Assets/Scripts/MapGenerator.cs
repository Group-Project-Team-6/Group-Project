using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using TMPro;
using UnityEngine;

public class MapGenerator : MonoBehaviour
{
    public MazeGenerator mazeGen;
    public CollectableGenerator collGen;
    public PlayerSpawner pSpawner;
    int[] playerCount = {0,0,0,0};
    public ModuleMap mMap;
    public int unitLength = 10;
    public int width;
    public int length;
    List<string> maze;
    List<GameObject> Q;
    GameObject floor;
    GameObject wall1;
    GameObject wall2;
    GameObject wall3;
    GameObject wall4;
    public TextMeshPro tmpp;

    // Start is called before the first frame update
    private void Awake()
    {
        Q = new List<GameObject>(4);
        Q.Add(transform.Find("Q1").gameObject);
        Q.Add(transform.Find("Q2").gameObject);
        Q.Add(transform.Find("Q3").gameObject);
        Q.Add(transform.Find("Q4").gameObject);
        floor = transform.Find("Ground").gameObject;
        wall1 = transform.Find("Wall1").gameObject;
        wall2 = transform.Find("Wall2").gameObject;
        wall3 = transform.Find("Wall3").gameObject;
        wall4 = transform.Find("Wall4").gameObject;
    }
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
        LoadMazeFromList();
        InitBackgrounds();
        for (int i = 0; i < 4; i++)
        {
            for (int level = 0; level < maze.Count; level++)
            {
                for (int l = 0; l < length; l++)
                {
                    for (int w = 0; w < width; w++)
                    {
                        GameObject cell = AddChild(i, GetSymbol(level, l, w), level, l, w);
                        if (playerCount[i] < 1 && l == length - 1 && level == 0 && i % 2 == 0 && GetSymbol(level, l, w) == 'P')
                        {
                            GameManager.gameManager.players.Add(pSpawner.Spawn(i/2, cell.transform.position, Q[i].transform));
                            playerCount[i]++;
                        }
                    }
                }
                collGen.Collectables(level, maze[level], width, length, unitLength,Q[i].transform);
            }
        }
    }

    void InitBackgrounds()
    {
        floor.transform.localScale = new Vector3(length * unitLength * 0.2f, 1, width * unitLength * 0.2f);

        wall1.transform.localScale = new Vector3(length * unitLength * 2.0f, maze.Count * unitLength, 2);
        wall1.transform.localPosition = new Vector3(0, maze.Count * unitLength * 0.5f - 1.5f, width * unitLength + 1.0f);
        wall2.transform.localScale = new Vector3(length * unitLength * 2.0f, maze.Count * unitLength, 2);
        wall2.transform.localPosition = new Vector3(0, maze.Count * unitLength * 0.5f - 1.5f, -width * unitLength - 1.0f);
        wall3.transform.localScale = new Vector3(2, maze.Count * unitLength, length * unitLength * 2.0f);
        wall3.transform.localPosition = new Vector3(-length * unitLength - 1.0f, maze.Count * unitLength * 0.5f - 1.5f, 0);
        wall4.transform.localScale = new Vector3(2, maze.Count * unitLength, length * unitLength * 2.0f);
        wall4.transform.localPosition = new Vector3(length * unitLength + 1.0f, maze.Count * unitLength * 0.5f - 1.5f, 0);
    }

    private char GetSymbol(int level, int l, int w)
    {
        //Debug.Log(level + " " + (l * width + w));
        return maze[level][l * width + w];
    }

    private void LoadMazeFromList()
    {
        maze = mazeGen.GetLevelStrings();
    }

    private void LoadMazeFromTxt()
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

    private GameObject AddChild(int i, char symbol, int level, int l, int w)
    {
        if (symbol == 'S' || !mMap.moduleMap.ContainsKey(symbol)) return null;
        GameObject g = Instantiate(
            mMap.bank[mMap.moduleMap[symbol]],
            new Vector3(0, 0,0),
            new Quaternion(), Q[i].transform);
        g.transform.localScale = new Vector3(unitLength, unitLength, unitLength);
        g.transform.localPosition = new Vector3(((float)l + 0.5f) * unitLength, level * unitLength, ((float)w + 0.5f) * unitLength);
        return g;
    }

    private void DeleteChild()
    {
        for(int i = transform.childCount - 1; i >= 0; i--)
        {
            Destroy(transform.GetChild(i).gameObject);
        }
    }
}
