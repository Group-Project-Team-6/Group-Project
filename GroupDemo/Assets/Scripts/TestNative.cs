using UnityEngine;
using System.Runtime.InteropServices;
public class TestNative : MonoBehaviour
{
    // The imported function
    [DllImport("TestUnity2")]
    public static extern void Test([Out] byte[] c, int a);

    void Start()
    {
        byte[] c = new byte[4];
        Test(c, 4);
        for(int i = 0; i < c.Length; i++)
        {
            Debug.Log(c[i]);
        }
        
    }
}