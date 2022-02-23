using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using UnityEngine;

/// <summary>
/// <>
/// </summary>
public class UDPMessageEventArgs : EventArgs
{
    public byte[] bytes;
}

public class CommandMessage 
{
    public byte command;
}


public class NetworkManager : MonoBehaviour
{
    static UdpClient client = new UdpClient(666);
    public byte[] ipAddress = { 0, 0, 0, 0 };
    static IPEndPoint endPoint;
    static IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 666);

    public static Queue<CommandMessage> cmdQueue;

    Task t = new Task(Listen);
    static public event EventHandler<UDPMessageEventArgs> OnMessageRecieved;

    static NetworkManager instance;
    static bool alive;
    void Awake()
    {
        endPoint = new IPEndPoint(new IPAddress(ipAddress), 666);
        if (!instance)
        {
            instance = this;
            alive = true;
            OnMessageRecieved += OnRecievedMsg;
        }
        else
        {
            Destroy(this);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        t.Start();
    }

    // Update is called once per frame
    void Update()
    {
        for (int i = 0; i < cmdQueue.Count; i++)
        {
            byte[] bytes = ResovleCmd(cmdQueue.Dequeue());
            if(bytes.Length > 0)
            {
                Send(bytes);
            }
        }
    }

    private void OnDestroy()
    {
        alive = false;
        Disconnect();
    }
    static void Listen()
    {
        byte[] bytes = {0 };
        client.Client.ReceiveTimeout = 2000;

        while (alive)
        {
            Debug.Log("Start Listening");
            bytes = client.Receive(ref RemoteIpEndPoint);
            Debug.Log("Recieved: " + bytes[0]);

            if (bytes.Length < 1) continue;

            UDPMessageEventArgs args = new UDPMessageEventArgs();
            args.bytes = bytes;
            if(OnMessageRecieved != null) OnMessageRecieved.Invoke(null, args);
        }
    }

    static void Send(byte[] n)
    {
        Debug.Log("Start Sending");
        byte[] sendBytes = n;//System.Text.Encoding.ASCII.GetBytes("Is anybody there?");
        client.Send(sendBytes, sendBytes.Length);
    }

    byte[] ResovleCmd(CommandMessage e)
    {
        byte[] bytes = { e.command };
        return bytes;
    }

    /// <summary>
    /// Operation to do when message recieved
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="args"></param>
    static void OnRecievedMsg(object sender, UDPMessageEventArgs args)
    {

    }

    /// <summary>
    /// Pass game command here
    /// </summary>
    /// <param name="n"></param>
    public static void PassCmd(CommandMessage n)
    {
        cmdQueue.Enqueue(n);
    }

    public static void Connect(IPEndPoint endPoint)
    {
        Debug.Log(endPoint.Address.ToString() + endPoint.Port.ToString());
        client.Connect(endPoint);
    }

    public static void Disconnect()
    {
        client.Close();
    }
}
