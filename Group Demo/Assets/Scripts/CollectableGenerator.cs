using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CollectableGenerator : MonoBehaviour
{
    static CollectableGenerator instance;
    string mapChars;
    char[,] map2DArray;
    int[,] positions;
    public static int numCollectables = 5;
    public GameObject collectable;

    // Start is called before the first frame update
    void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            print("Duplicated collectableGenerator");
        }
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void Collectables(int level, string map, int sizeX, int sizeY, float unitLength, Transform Q)
    {
        findPositions(map, sizeX, sizeY);
        spawnCollectables(level, unitLength, Q);
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

        //int numCollectables = Mathf.Max(1, (int)((sizeX * sizeY) * 0.05f));
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
        Debug.Log(positions.Length);

    }

    void spawnCollectables(int level, float unitLength, Transform t)
    {
        for (int i = 0; i < numCollectables; i++)
        {
            Instantiate(collectable,
                new Vector3(0, 0, 0),
                Quaternion.identity,
                t)
                .transform.localPosition = new Vector3(
                    (0.5f + (float)positions[i, 1]) * unitLength,
                    level * unitLength,
                    (0.5f + (float)positions[i, 0]) * unitLength);
        }
    }

}
