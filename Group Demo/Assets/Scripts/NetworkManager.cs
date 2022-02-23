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

public class CommandMessage 
{
    byte from;
    byte command;

}


public class NetworkManager : MonoBehaviour
{
    static UdpClient client = new UdpClient(666);
    public byte[] ipAddress = { 0, 0, 0, 0 };
    static IPEndPoint endPoint;
    static IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 666);
    Task t = new Task(Listen);
    static public event EventHandler<UDPMessageEventArgs> onMessageRecieved;
    static NetworkManager instance;
    void Awake()
    {
        endPoint = new IPEndPoint(new IPAddress(ipAddress), 666);
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
        Debug.Log(endPoint.Address.ToString() + endPoint.Port.ToString());
        client.Connect(endPoint);
        t.Start();
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log(t.IsCompleted);
        Send(2);
    }

    private void OnDestroy()
    {
        client.Close();
    }
    static void Listen()
    {
        byte[] bytes = {0 };
        while (bytes[0] != 3)
        {
            Debug.Log("Start Listening");
            bytes = client.Receive(ref RemoteIpEndPoint);
            Debug.Log("Recieved: " + bytes[0]);
            UDPMessageEventArgs args = new UDPMessageEventArgs();
            args.num = bytes[0];
            if(onMessageRecieved != null) onMessageRecieved.Invoke(null, args);
        }
    }

    static void Send(byte n)
    {
        Debug.Log("Start Sending");
        byte[] sendBytes = { n };//System.Text.Encoding.ASCII.GetBytes("Is anybody there?");
        client.Send(sendBytes, sendBytes.Length);
    }

    static void StartServer()
    {

    }

    static void JoinServer()
    {

    }
}
