using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

public class GameServer : MonoBehaviour
{
    List<Vector3> playerSlost;
    List<string> maze;
    List<Vector3> collectablePos;
    List<IPEndPoint> clients;

    void Init()
    {
        //GenMaze / grab generated maze
        //Assign position slot
        
    }

    void SyncResourses()
    {
        //Maze 
        //player slots
        //
    }

    void AddUser()
    {

    }

    byte[] ExportMaze()
    {
        byte[] bytes = { };

        return bytes;
    }
}
