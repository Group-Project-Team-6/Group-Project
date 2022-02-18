using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PaintableWall : MonoBehaviour
{
    //public RenderTexture hitTex;
    public bool LoadOnAwake = false;
    int SplashWidth;
    int SplashHeight;
    public float PlaneHeight = 10;
    public float PlaneWidth = 10;
    public List<Texture2D> tex;
    public List<Texture2D> texOri;
    public int Layer;
    Mesh mesh;
    // Start is called before the first frame update
    void Awake()
    {
        mesh = GetComponent<MeshFilter>().mesh;
        if (LoadOnAwake)
        {
            TexInit();
        }
    }

    // Update is called once per frame
    void Update()
    {

    }

    void TexInit()
    {
        if (tex.Count < 1)
        {

            Debug.Log("Enter TexInit: " + gameObject.name + mesh.subMeshCount);
            float WidthRatio = PlaneHeight > PlaneWidth ? PlaneWidth / PlaneHeight : 1;
            float HeightRatio = PlaneHeight > PlaneWidth ? 1 : PlaneHeight / PlaneWidth;
            //hitTex = new Texture2D(256, 256);
            for (int i = 0; i < mesh.subMeshCount; i++)
            {
                tex.Add(new Texture2D((int)(64 * WidthRatio), (int)(64 * HeightRatio)));
                transform.GetComponent<Renderer>().materials[i].SetTexture("_Tex", tex[i]);
                int index = (i >= texOri.Count ? i = texOri.Count - 1 : i);
                transform.GetComponent<Renderer>().materials[i].SetTexture("_TexOri", texOri[index]);
                Color32[] background = new Color32[tex[i].width * tex[i].height];
                for (int j = 0; j < (tex[i].width * tex[i].height); j++) background[j] = new Color32(0, 0, 0, 0);
                tex[i].SetPixels32(0, 0, tex[i].width, tex[i].height, background);
                tex[i].Apply();
            }
        }

    }

    void OnCollisionEnter(Collision collision)
    {
        try
        {
            if (collision.gameObject.GetComponent<SplashBall>())
            {
                TexInit();
                SplashBall ball = collision.gameObject.GetComponent<SplashBall>();
                gameObject.layer = Layer;
                if (SplashManager.approach == 1)
                {
                    int mIndex = 0;
                    Vector2 uv = GetUV(collision, ref mIndex);
                    Color32[] c = ball.tex.GetPixels32();// new Color32[SplashWidth * SplashHeight];
                    SplashHeight = ball.tex.height;
                    SplashWidth = ball.tex.width;
                    Color32 baseColor = collision.gameObject.GetComponent<SplashBall>().color;
                    int x = (int)(uv.x * (tex[mIndex].width - 1));
                    int y = (int)(uv.y * (tex[mIndex].height - 1));
                    int halfX = x - SplashWidth / 2;
                    int halfY = y - SplashHeight / 2;
                    Debug.Log(halfY);
                    Debug.Log(halfX);
                    for (int v = 0; v < SplashHeight; v++)
                    {
                        for (int u = 0; u < SplashWidth; u++)
                        {
                            int i = v * SplashWidth + u;
                            int posX = u + halfX;
                            int posY = v + halfY;
                            if (posX > tex[mIndex].width - 1 || posX < 0 || posY > tex[mIndex].height - 1 || posY < 0) continue;
                            if (c[i].a > 0.7)
                            {
                                float factor = ((float)u / (float)SplashWidth) * ((float)v / (float)SplashHeight);
                                tex[mIndex].SetPixel(posX, posY, new Color32(
                                    (byte)(factor * (float)baseColor.r),
                                    (byte)(factor * (float)baseColor.b),
                                    (byte)(factor * (float)baseColor.g),
                                    baseColor.a));
                            }

                        }
                    }
                    tex[mIndex].Apply();
                    //int w = tex.width - 1;
                    //int h = tex.height - 1;
                    //if (Mathf.Min(SplashWidth, w - (int)(uv.x * w)) != 0 && Mathf.Min(SplashHeight, h - (int)(uv.y * h)) != 0)
                    //{
                    //    int x = (int)(uv.x * w);
                    //    int y = (int)(uv.y * h);
                    //    int xFirstHalf = Mathf.Min(x, SplashWidth/2);  //[SplashWidth / 2   ->   SplashWidth]
                    //    int yFirstHalf = Mathf.Min(y, SplashHeight/2); //[SplashHeight / 2   ->   SplashHeight]
                    //    int xSecondHalf = Mathf.Min(SplashWidth - SplashWidth / 2, w - x + 1);
                    //    int ySecondHalf = Mathf.Min(SplashHeight - SplashHeight / 2, h - y + 1);
                    //    tex.SetPixels32(Mathf.Max(0,x - (SplashWidth/2)), Mathf.Max(0, y - SplashHeight/2), xFirstHalf + xSecondHalf, yFirstHalf + ySecondHalf, c);
                    //    tex.Apply();
                    //}
                    //transform.GetComponent<Renderer>().material.SetTexture("_MainTex", hitTex);
                }
                else if (SplashManager.approach == 2)
                {
                    Splash s = new Splash();
                    s.mass = collision.gameObject.GetComponent<Rigidbody>().mass;
                    s.position = collision.contacts[0].point;
                    s.halfSize = collision.gameObject.GetComponent<Collider>().bounds.size.magnitude;

                    SplashManager.splashes.Add(s);
                }
            }
        }
        catch (System.Exception e)
        {
            Debug.Log(e.Message);
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


    private void FindTriangle(Collision collision, ref int[] shortestTri, ref Vector3 proj, ref float[] triRatio, ref int subMeshNum)
    {
        float shortestDist = float.MaxValue;
        Vector3 p = collision.GetContact(0).point;
        Debug.DrawLine(p, p - collision.GetContact(0).normal * 100, Color.green, 10.0f);
        for (int mIndex = 0; mIndex < mesh.subMeshCount; mIndex++)
        {
            int[] tris = mesh.GetTriangles(mIndex);
            for (int i = 0; i < tris.Length; i += 3)
            {
                Vector3 a = gameObject.transform.localToWorldMatrix * ToVector4(mesh.vertices[tris[i]]);
                Vector3 b = gameObject.transform.localToWorldMatrix * ToVector4(mesh.vertices[tris[i + 1]]);
                Vector3 c = gameObject.transform.localToWorldMatrix * ToVector4(mesh.vertices[tris[i + 2]]);

                Vector3 ac = c - a;
                Vector3 ab = b - a;

                Vector3 pa = p - a;

                Vector3 cross = Vector3.Cross(ac, ab); // Normal            
                Vector3 n = cross.normalized;  // Noraml normalized
                if (!(Vector3.Dot(n, collision.GetContact(0).normal) > 0.7f)) continue;

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
                if (Mathf.Abs(A - (aA + bA + cA)) > 0.01f) continue;
                //Debug.Log(A + " " + aA + " "+ bA + " " + cA + " " + (A != (aA + bA + cA)));
                //Debug.Log("Dist:" + "p" + p + "  r: " + r + " - " + (p - r).magnitude + " " + shortestDist);
                if ((p - r).magnitude < shortestDist)
                {

                    shortestDist = (p - r).magnitude;
                    shortestTri[2] = tris[i + 2];
                    shortestTri[1] = tris[i + 1];
                    shortestTri[0] = tris[i];
                    triRatio[2] = aA / A;
                    triRatio[1] = cA / A;
                    triRatio[0] = bA / A;
                    proj = r;
                    subMeshNum = mIndex;
                }
            }
        }
    }

    private Vector2 GetUV(Collision collision, ref int mIndex)
    {
        int[] shortestTri = new int[3];
        float[] triRatio = new float[3];
        Vector3 proj = new Vector3();

        FindTriangle(collision, ref shortestTri, ref proj, ref triRatio, ref mIndex);

        Vector2 hitUV;

        //Debug.Log("Tris:" + shortestTri[0] + " " + shortestTri[1] + " " + shortestTri[2]);
        //Debug.Log("UVs:" + m.uv[shortestTri[0]] + " " + m.uv[shortestTri[1]] + " " + m.uv[shortestTri[2]]);

        //Vector3 aa = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        //Vector3 bb = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        //Vector3 cc = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);

        //Vector3 tangent = (bb - aa).normalized;
        //Vector3 normal = Vector3.Cross(cc - aa, tangent).normalized;
        //Vector3 cotangent = Vector3.Cross(normal, tangent).normalized;

        //Vector2 planeC = new Vector2(Vector3.Dot(cc - aa,cotangent), Vector3.Dot(cc - aa, tangent));
        //Vector2 planeB = new Vector2(Vector3.Dot(bb - aa, cotangent), Vector3.Dot(bb - aa, tangent));
        //Vector2 planeA = new Vector2(0, 0);
        //Vector2 planeR = new Vector2(Vector3.Dot(proj - aa, cotangent), Vector3.Dot(proj - aa, tangent));

        //Debug.Log(planeA + " " + planeB + " " + planeC + " " + planeR);

        //float yamyb = (planeA.y - planeB.y);
        //float xrmxb = planeR.x - planeB.x;
        //float yrmyb = planeR.y - planeB.x;
        //float denorm = (yamyb * (planeC.x - planeB.x)) + ((planeB.x - planeA.x) * (planeC.y - planeB.y));

        //float Rc = (yamyb * xrmxb + (planeB.x - planeA.x) * yrmyb) / denorm;
        //float Ra = Mathf.Abs(((planeB.y - planeC.y) * xrmxb + (planeC.x - planeB.x) * yrmyb) / denorm);
        //float Rb = 1 - Rc - Ra;

        //3D Approach (!Failed)
        //Vector3 AC = cc - aa;
        //Vector3 AR = proj - aa;
        //float Rc = Ratio(Proj(AR, AC), AC);

        //Vector3 BC = cc - bb;
        //Vector3 BR = proj - bb;
        //float Rb = Ratio(BR - Proj(BR, BC), bb - aa);
        //float Ra = 1.0f - Rc - Rb;

        Debug.Log(mesh.uv[shortestTri[0]] * triRatio[0] + " " + mesh.uv[shortestTri[1]] * triRatio[1] + " " + mesh.uv[shortestTri[2]] * triRatio[2]);
        hitUV = mesh.uv[shortestTri[0]] * triRatio[0] + mesh.uv[shortestTri[1]] * triRatio[1] + mesh.uv[shortestTri[2]] * triRatio[2];
        Debug.Log(hitUV.x + " " + hitUV.y);
        //Vector3 f = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[0]]);
        //Vector3 h = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[1]]);
        //Vector3 k = gameObject.transform.localToWorldMatrix * ToVector4(m.vertices[shortestTri[2]]);
        //GameObject g = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //g.transform.position = new Vector3(f.x, f.y, f.z); ;
        //GameObject gg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //gg.transform.position = new Vector3(h.x, h.y, h.z);
        //GameObject ggg = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        //ggg.transform.position = new Vector3(k.x, k.y, k.z);
        return hitUV;

        //Debug.Log("Splash Pos: "+ p);
        //Debug.Log("Vertex a: " + shortestTri[0] + ", Vertex b: " + shortestTri[1] + ",Vertex c: " + shortestTri[2]);
    }
}
