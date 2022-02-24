using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

public class GameServer : MonoBehaviour
{
    public MazeGenerator mazeGen;
    public MapGenerator mapGen;
    List<Vector3> playerSlost;
    List<string> maze;
    List<Vector3> collectablePos;
    List<IPEndPoint> clients;
    float timeRefernce;

    private void Update()
    {
        timeRefernce += Time.deltaTime;
    }

    void Init()
    {
        mazeGen.Generate(mapGen.length,mapGen.width);
        maze = mazeGen.GetLevelStrings();
        //Assign position slot

    }

    void SyncResourses()
    {
        timeRefernce = 0.0f;
        //Maze 
        //Collectable position
        //player slots

    }

    void AddUser()
    {
        //Get user ip
        //assign player ID (start from 0)
        
        
    }

    byte[] ExportMaze()
    {
        byte[] bytes = { };

        return bytes;
    }

    void OnMessageRecieved(object sender, UDPMessageEventArgs args)
    {
        //Send the Msg to all client
        foreach(IPEndPoint client in clients)
        {
            NetworkManager.Connect(client);
            NetworkManager.Send(args.bytes);
        }

    }
}
