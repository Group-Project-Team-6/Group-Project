using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Splash : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnCollisionEnter(Collision collision)
    {
        //Vector3 shortestCentre = new Vector3(float.MaxValue,float.MaxValue,float.MaxValue);
        float shortestDist = float.MaxValue;
        int[] shortestTri = new int[3];
        Vector2 hitUV = new Vector2(0, 0);
        Mesh m = collision.gameObject.GetComponent<MeshFilter>().mesh;
        Vector3 p = collision.contacts[0].point;
        Vector3 proj = new Vector3();

        for (int i = 0; i < m.triangles.Length; i+=3)
        {
            Vector3 a = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i]]);
            Vector3 b = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i + 1]]);
            Vector3 c = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i + 2]]);
           
            Vector3 ac = c - a;
            Vector3 ab = b - a;

            Vector3 pa = p - a;

            Vector3 cross = Vector3.Cross(ac, ab); // Normal
            Vector3 n = -cross.normalized;  // Noraml normalized
            Vector3 r = pa - (Vector3.Dot(pa, n) * n) + a;  // vector from origin to projection from p to triangle plane
            Vector3 ra = a - r;
            Vector3 rb = b - r;
            Vector3 rc = c - r;

            float A = cross.magnitude; //// total Area
            float aA = Vector3.Cross(ra, rb).magnitude; // divided Area
            float bA = Vector3.Cross(rb, rc).magnitude;// divided Area
            float cA = Vector3.Cross(rc, ra).magnitude;// divided Area

            //Debug.Log("i: " + i + "=================");
            //Debug.Log(A + " " + (aA + bA + cA));
            //Debug.Log((p - r).magnitude + " " + shortestDist);
            if (A != (aA + bA + cA)) continue;
            if ((p-r).magnitude < shortestDist)
            {
                shortestDist = (p - r).magnitude;
                shortestTri[2] = m.triangles[i + 2];
                shortestTri[1] = m.triangles[i + 1];
                shortestTri[0] = m.triangles[i];
                proj = r;
            }           
        }
        Debug.Log(m.uv[shortestTri[0]] + " " + m.uv[shortestTri[1]] + " " + m.uv[shortestTri[2]]);
        Vector3 aa = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        Vector3 bb = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        Vector3 cc = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);

        Vector3 Al = (bb - aa).normalized * (Vector3.Dot((bb - aa).normalized, (proj - aa)));
        Vector3 lR = (proj - aa) - Al;
        Vector3 Al2 = (bb - aa).normalized * (Vector3.Dot((bb - aa).normalized, (cc - aa)));
        Vector3 l2C = (cc - aa) - Al2;
        float Rc = lR.magnitude / l2C.magnitude;
        float Rb = ((proj - bb) - (((cc - bb).magnitude * Rc) * (cc - bb).normalized)).magnitude / (bb - aa).magnitude;
        float Ra = 1 - Rc - Rb;
        Debug.Log(Ra + " " + Rb + " " + Rc);
        hitUV = m.uv[shortestTri[0]] * Ra + m.uv[shortestTri[1]] * Rb + m.uv[shortestTri[2]] * Rc;
        Debug.Log(hitUV);
        //Debug.Log("Splash Pos: "+ p);
        //Debug.Log("Vertex a: " + shortestTri[0] + ", Vertex b: " + shortestTri[1] + ",Vertex c: " + shortestTri[2]);
        //Vector3 f = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        //Vector3 h = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        //Vector3 k = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);
        //GameObject g = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //g.transform.position = new Vector3(f.x, f.y, f.z);
        //GameObject gg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //gg.transform.position = new Vector3(h.x, h.y, h.z);
        //GameObject ggg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //ggg.transform.position = new Vector3(k.x, k.y, k.z);

        Destroy(gameObject);
    }

    private Vector4 ToVector4(Vector3 v)
    {
        return new Vector4(v.x, v.y, v.z ,1);
    }

    void OnCollisionStay(Collision collision)
    {
        Destroy(gameObject);
    }
}
