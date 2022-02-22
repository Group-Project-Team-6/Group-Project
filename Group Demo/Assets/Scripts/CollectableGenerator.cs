using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CollectableGenerator : MonoBehaviour
{
    string mapChars;
    char[,] map2DArray;
    int[,] positions;
    public static int CollectableNumber = 20;
    public GameObject[] collectable = new GameObject[CollectableNumber];

    // Start is called before the first frame update
    void Start()
    {
     
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Collectables(int level, string map, int sizeX, int sizeY, float unitLength)
    {
        findPositions(map, sizeX, sizeY);
        spawnCollectables(level, unitLength);
    }

    void findPositions(string map, int sizeX, int sizeY)
    {
        map2DArray = new char[sizeX, sizeY];
        int i = 0;

        for (int y = 0; sizeY > y; y++)
        {
            for (int x = 0; sizeX > x; x++)
            {
                map2DArray[x, y] = map[i];
                i++;
            }
        }

        int numCollectables = ((sizeX * sizeY) / 50);
        positions = new int[numCollectables, 2];
        int placed = 0;

        while (placed < numCollectables)
        {
            int y = Random.Range(0, sizeY);
            int x = Random.Range(0, sizeX);

            if (map2DArray[x, y] == 'P')
            {
                positions[placed, 0] = x;
                positions[placed, 1] = y;
                placed++;
            }
        }

    }

    void spawnCollectables(int level, float unitLength)
    {
        float translate = 0;

        for (int i = 0; i < positions.GetLength(0); i++)
        {
            //Instantiate(collectable, new Vector3(positions[i, 0] *  (unitLength * 0.5f), level * (unitLength * 0.5f), positions[i, 1] *  (unitLength * 0.5f)), Quaternion.identity,this.transform);
        }
    }

}
