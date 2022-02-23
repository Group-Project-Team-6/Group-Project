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
    void SyncResourses()
    {
        CommandMessage m = new CommandMessage();
        m.command = 7;
        NetworkManager.PassCmd(m);
    }

    void AddUser()
    {
        NetworkManager.SendData(ExportMaze());
    }

    byte[] ExportMaze()
    {
        byte[] bytes = { };

        return bytes;
    }
}
