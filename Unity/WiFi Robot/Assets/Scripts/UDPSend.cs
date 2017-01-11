/*
 
    -----------------------
    UDP-Send
    -----------------------
    // [url]http://msdn.microsoft.com/de-de/library/bb979228.aspx#ID0E3BAC[/url]
   
    // > gesendetes unter
    // 127.0.0.1 : 8050 empfangen
   
    // nc -lu 127.0.0.1 8050
 
        // todo: shutdown thread at the end
*/
using UnityEngine;
 
using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Timers;
 
public static class UDPSend
{
    private const int DELAY = 200;
    private static int localPort;

    // Timer to continue to send the UDP directive, will change each button press
    private static Timer _timer;
   
    private static int _port;
    private static string _ip;
   
    // "connection" things
    static IPEndPoint remoteEndPoint;
    static UdpClient client;

    public static void SetUp(string ip, int port) {
        _ip = ip;
        _port = port;
        init();
    }
   
    // init
    private static void init()
    {
        remoteEndPoint = new IPEndPoint(IPAddress.Parse(_ip), _port);
        client = new UdpClient();
       
        // status
        Debug.Log("Sending to " + _ip + " : " + _port);
        Debug.Log("Testing: nc -lu " + _ip + " : " + _port);
    }
 
    // sendData
    public static void sendString(string message)
    {
        try
        {
            byte[] data = Encoding.UTF8.GetBytes(message);
 
            client.Send(data, data.Length, remoteEndPoint);
        }
        catch (Exception err)
        {
            Debug.Log(err.ToString());
        }
    }
   
    // endless test
    public static void setUDPDirective(string testStr)
    {
        stopSending();

        _timer = new Timer(DELAY);
        _timer.Elapsed += (source, args)=>{
            sendString(testStr);
        };
        _timer.AutoReset = true;

        _timer.Enabled = true;
        _timer.Start();
    }

    public static void stopSending() {
        if (_timer != null && _timer.Enabled) {
            _timer.Stop();
            _timer.Enabled = false;
            _timer.Dispose();
        }
    }
   
}