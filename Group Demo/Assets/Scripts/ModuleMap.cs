using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class ModuleMap : MonoBehaviour
{
    public List<GameObject> bank;
    public List<char> moduleMapList;
    public Dictionary<char, int> moduleMap;
    public bool writeModuleMap = false;


    void Start()
    {
        moduleMapList = new List<char>();
        moduleMap = new Dictionary<char, int>();
    }

    // Update is called once per frame
    void Update()
    {
        if (writeModuleMap)
        {
            WriteModuleMap();
            writeModuleMap = false;
        }
    }

    public void LoadModuleMap()
    {
        moduleMapList = new List<char>();
        moduleMap = new Dictionary<char, int>();
        string path = Path.Combine(Application.dataPath, "moduleMap.txt");
        StreamReader sr = new StreamReader(path);
        string strLine = sr.ReadLine();
        int index = 0;
        while (strLine != null)
        {
            if (index < bank.Count)
            {
                moduleMapList.Add(strLine[0]);
                moduleMap.Add(strLine[0], index);
            }
            strLine = sr.ReadLine();
            index++;

        }
        sr.Close();
    }

    private void WriteModuleMap()
    {
        string path = Path.Combine(Application.dataPath, "moduleMap.txt");
        StreamWriter sw = new StreamWriter(path);
        for(int i = 0; i < moduleMapList.Count; i++)
        {
            sw.WriteLine(moduleMapList[i]);
        }
        sw.Close();
    }

}
