Shader "Custom/DefualtFloor"
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        //_MainTex("Albedo (RGB)", 2D) = "white" {}
        _Tex("Tex (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
    }
    SubShader{
        Tags{"Queue" = "Geometry"}
        pass {
            Tags{"LightMode" = "ForwardBase"}

            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            fixed4 _Color;
            //sampler2D _MainTex;
            sampler2D _Tex;

            struct a2v {
                float4 vertex : POSITION;
                float4 texcoord : TEXCOORD0;
            };

            struct v2f {
                float4 pos : SV_POSITION;
                float4 texcoord : TEXCOORD0;
                fixed4 color : COLOR;
            };

            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.texcoord = v.texcoord;
                o.color = _Color;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                //float3 wcoord = (i.srcPos.xyz / i.srcPos.w);
                fixed4 color = tex2D(_Tex,i.texcoord);
                if (color.a > 0.7) i.color = color;
                return i.color; //(1 - i.pos.z + 0.07)
            }

            ENDCG
        }
    }
    FallBack "Diffuse"
}
