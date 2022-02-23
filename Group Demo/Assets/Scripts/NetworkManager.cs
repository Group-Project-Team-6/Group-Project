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
    static UdpClient client = new UdpClient(666);
    public byte[] ipAddress = { 0, 0, 0, 0 };
    static IPEndPoint endPoint;
    static IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 5000);
    Task t = new Task(Listen);
    static public event EventHandler<UDPMessageEventArgs> onMessageRecieved;
    static NetworkManager instance;
    void Awake()
    {
        endPoint = new IPEndPoint(new IPAddress(ipAddress), 5000);
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

    private void OnDestroy()
    {
        client.Close();
    }
    static void Listen()
    {
        byte[] bytes = client.Receive(ref RemoteIpEndPoint);
        UDPMessageEventArgs args = new UDPMessageEventArgs();
        args.num = bytes[0];
        onMessageRecieved.Invoke(null,args);
    }

    static void Send()
    {
        byte[] sendBytes = System.Text.Encoding.ASCII.GetBytes("Is anybody there?");
        client.Send(sendBytes, sendBytes.Length, endPoint);
    }


}
