//Project: CSC8507 Group Project Unity Prototype
//Author: Chris Hui

Shader "Custom/DefaultWall" {
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        //_MainTex("Albedo (RGB)", 2D) = "white" {}
        _Tex("Tex (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
        _CullingFactor("CullingFactor", Float) = 1.0
        _CullingDistance("CullingDistance", Range(0,1)) = 0.08
    }
    SubShader{
        Tags{"Queue"="Transparent" "RenderType"="Transparent" "IgnoreProjector"="True"}
        pass {
            Tags{"LightMode"="ForwardBase"}
            Cull Off
            ZWrite Off
            Blend SrcAlpha OneMinusSrcAlpha
            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            fixed4 _Color;
            float _IsCasted;
            float _CullingFactor;
            fixed _CullingDistance;
            //sampler2D _MainTex;
            sampler2D _Tex;

            struct a2v {
                float4 vertex : POSITION;
                float4 texcoord : TEXCOORD0;
            };

            struct v2f {
                float4 pos : SV_POSITION;
                float4 texcoord : TEXCOORD0;
                float4 srcPos : TEXCOORD1;
                fixed4 color : COLOR;
            };

            v2f vert(a2v v){
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.texcoord = v.texcoord;// *2 - 1;
                o.srcPos = ComputeScreenPos(o.pos);
                o.color = _Color;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                //float3 wcoord = (i.srcPos.xyz / i.srcPos.w);
                fixed4 color = tex2D(_Tex,i.texcoord);
                if (color.x == 1.0) i.color = fixed4(0.0, 0.0, 0.0, 1.0);
                if(i.pos.z < _CullingDistance || _IsCasted == 0.0) return i.color; //(1 - i.pos.z + 0.07)
                float factor = (1 - i.pos.z + _CullingDistance);
                return  i.color * fixed4(1,1,1, clamp(pow(factor, _CullingFactor), 0.0, 1.0)); //(abs(wcoord.x-0.5) + abs(wcoord.y - 0.5)) *;
            }

            ENDCG            
        }
    }
    FallBack "Diffuse"
}
