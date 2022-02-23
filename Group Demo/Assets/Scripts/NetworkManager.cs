using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using UnityEngine;

public class UDPMessageEventArgs : EventArgs
{
    public int num;
}

public class NetworkManager : MonoBehaviour
{
    static UdpClient client = new UdpClient(5000);
    static IPEndPoint endPoint = new IPEndPoint(new IPAddress(new byte[4] { 192, 168, 0, 0 }),5000);
    static IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 5000);
    Task t = new Task(Listen);
    static public event EventHandler<UDPMessageEventArgs> onMessageRecieved;
    static NetworkManager instance;
    void Awake()
    {
        if (!instance)
        {
            instance = this;
        }
        else
        {
            Destroy(this);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        client.Connect(endPoint);
    }

    // Update is called once per frame
    void Update()
    {

        if (t.IsCompleted)
        {
            Send();
            t.Start();
        }
    }

    static void Listen()
    {
        byte[] bytes = client.Receive(ref RemoteIpEndPoint);
        UDPMessageEventArgs args = new UDPMessageEventArgs();
        args.num = 1;
        onMessageRecieved.Invoke(null,args);
    }

    static void Send()
    {

    }


}
