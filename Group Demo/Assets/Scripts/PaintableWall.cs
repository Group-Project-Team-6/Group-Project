using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PaintableWall : MonoBehaviour
{
    List<Texture> textures;
    // Start is called before the first frame update
    void Start()
    {
        textures = new List<Texture>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.GetComponent<SplashBall>())
        {
            if (SplashManager.approach == 1)
            {
                GetUV(collision);
            }
            else if (SplashManager.approach == 2)
            {
                gameObject.layer = 0;
                Splash s = new Splash();
                s.mass = collision.gameObject.GetComponent<Rigidbody>().mass;
                s.position = collision.contacts[0].point;
                s.halfSize = collision.gameObject.GetComponent<Collider>().bounds.size.magnitude;

                SplashManager.splashes.Add(s);
            }
        }
    }

    private Vector4 ToVector4(Vector3 v)
    {
        return new Vector4(v.x, v.y, v.z, 1);
    }

    private Vector3 Proj(Vector3 from, Vector3 to)
    {
        return (to.normalized * (Vector3.Dot(to.normalized, from)));
    }

    private float Ratio(Vector3 numerator, Vector3 denorminator)
    {
        return numerator.magnitude / denorminator.magnitude;
    }


    private void FindTriangle(Collision collision, ref int[] shortestTri, ref Vector3 proj)
    {
        float shortestDist = float.MaxValue;
        Mesh m = gameObject.GetComponent<MeshFilter>().mesh;
        Vector3 p = collision.GetContact(0).point;

        Debug.DrawLine(p, p - collision.GetContact(0).normal * 100, Color.green, 10.0f);

        for (int i = 0; i < m.triangles.Length; i += 3)
        {
            Vector3 a = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i]]);
            Vector3 b = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i + 1]]);
            Vector3 c = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[m.triangles[i + 2]]);

            Vector3 ac = c - a;
            Vector3 ab = b - a;

            Vector3 pa = p - a;

            Vector3 cross = Vector3.Cross(ac, ab); // Normal            
            Vector3 n = cross.normalized;  // Noraml normalized
            if (!(Vector3.Dot(n, -collision.GetContact(0).normal) < -0.8f)) continue;
            Vector3 r = pa - (Vector3.Dot(pa, n) * n) + a;  // vector from origin to projection from p to triangle plane

            Debug.DrawLine(r, r + cross.normalized * 10, Color.magenta, 10.0f);

            Vector3 ra = a - r;
            Vector3 rb = b - r;
            Vector3 rc = c - r;

            float A = cross.magnitude; //// total Area
            float aA = Vector3.Cross(ra, rb).magnitude; // divided Area
            float bA = Vector3.Cross(rb, rc).magnitude;// divided Area
            float cA = Vector3.Cross(rc, ra).magnitude;// divided Area

            //Debug.Log("i: " + i + "=================");
            //Debug.Log(A + " " + (aA + bA + cA) + " " + (A != (aA + bA + cA)));
            if (Mathf.Abs(A - (aA + bA + cA)) > 0.01f) continue;
            //Debug.Log("Dist:" + "p" + p + "  r: " + r + " - " + (p - r).magnitude + " " + shortestDist);
            if ((p - r).magnitude < shortestDist)
            {

                shortestDist = (p - r).magnitude;
                shortestTri[2] = m.triangles[i + 2];
                shortestTri[1] = m.triangles[i + 1];
                shortestTri[0] = m.triangles[i];
                proj = r;
            }
        }
    }

    private Vector2 GetUV(Collision collision)
    {
        int[] shortestTri = new int[3];
        Vector3 proj = new Vector3();

        FindTriangle(collision, ref shortestTri, ref proj);

        Mesh m = gameObject.GetComponent<MeshFilter>().mesh;
        Vector2 hitUV;

        //Debug.Log("Tris:" + shortestTri[0] + " " + shortestTri[1] + " " + shortestTri[2]);
        //Debug.Log("UVs:" + m.uv[shortestTri[0]] + " " + m.uv[shortestTri[1]] + " " + m.uv[shortestTri[2]]);

        Vector3 aa = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        Vector3 bb = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        Vector3 cc = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);

        Vector3 AC = cc - aa;
        Vector3 AR = proj - aa;
        float Rc = Ratio(Proj(AR, AC), AC);

        Vector3 BC = cc - bb;
        Vector3 BR = proj - bb;
        float Rb = Ratio(BR - Proj(BR, BC), bb - aa);
        float Ra = 1.0f - Rc - Rb;
        Debug.Log(Ra + " " + Rb + " " + Rc);
        hitUV = m.uv[shortestTri[0]] * Ra + m.uv[shortestTri[1]] * Rb + m.uv[shortestTri[2]] * Rc;
        Debug.Log(hitUV.x + " " + hitUV.y);

        return hitUV;

        //Debug.Log("Splash Pos: "+ p);
        //Debug.Log("Vertex a: " + shortestTri[0] + ", Vertex b: " + shortestTri[1] + ",Vertex c: " + shortestTri[2]);
        //Vector3 f = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        //Vector3 h = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        //Vector3 k = collision.gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);

        //GameObject g = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //g.transform.position = new Vector3(f.x, f.y, f.z); ;
        //GameObject gg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //gg.transform.position = new Vector3(h.x, h.y, h.z);
        //GameObject ggg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //ggg.transform.position = new Vector3(k.x, k.y, k.z);
    }
}
