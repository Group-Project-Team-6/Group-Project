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

public class NetworkManager : MonoBehaviour
{
    static UdpClient client;
    public byte[] ipAddress = { 0, 0, 0, 0 };
    public static int port = 666;

    Task t = new Task(Listen);
    public static event EventHandler<UDPMessageEventArgs> OnMessageRecieved;

    public static NetworkManager instance;
    static bool alive;
    void Awake()
    {
        if (!instance)
        {
            instance = this;
            alive = true;
            client = new UdpClient(port);
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
        //Connect(endPoint);
        t.Start();
    }

    private void OnDestroy()
    {
        alive = false;
        DestroyClient();
    }

    static void Listen()
    {
        byte[] bytes = {0 };
        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, port);
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

    public static void Send(byte[] n)
    {
        Debug.Log("Start Sending");
        byte[] sendBytes = n;//System.Text.Encoding.ASCII.GetBytes("Is anybody there?");
        client.Send(sendBytes, sendBytes.Length);
    }

    /// <summary>
    /// Operation to do when message recieved
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="args"></param>
    static void OnRecievedMsg(object sender, UDPMessageEventArgs args)
    {

    }

    static void DestroyClient()
    {
        Connect(new IPEndPoint(IPAddress.Loopback, 666));
        Send(new byte[] { 3 });
        client.Close();
    }


    public static void Connect(IPEndPoint endPoint)
    {
        endPoint.Port = port;
        Debug.Log(endPoint.Address.ToString() + endPoint.Port.ToString());
        client.Connect(endPoint);
    }

    public static void Reset(int newPort)
    {
        client.Close();
        client = new UdpClient(newPort);
        port = newPort;
    }
}
