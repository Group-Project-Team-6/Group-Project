// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'


Shader "Custom/DefaultWall" {
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        _MainTex("Albedo (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
        _IsCasted("IsCasted", Float) = 0.0
    }
    SubShader{
        Tags{"Queue"="Transparent" "RenderType"="Transparent" "IgnoreProjector"="True"}
        pass {
            Tags{"LightMode"="ForwardBase"}

            ZWrite Off
            Blend SrcAlpha OneMinusSrcAlpha
            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            fixed4 _Color;
            float _isCasted;

            struct a2v {
                float4 vertex : POSITION;
                float4 texcoord : TEXCOORD0;
            };

            struct v2f {
                float4 pos : SV_POSITION;
                float4 srcPos : TEXCOORD0;
                fixed4 color : COLOR;
            };

            v2f vert(a2v v){
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.srcPos = ComputeScreenPos(o.pos);
                o.color = _Color;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                float3 wcoord = (i.srcPos.xyz / i.srcPos.w);
                if(i.pos.z < 0.12 ) return i.color; //(1 - i.pos.z + 0.07)
                return fixed4(i.color.rgb,clamp((abs(wcoord.x-0.5) + abs(wcoord.y - 0.5)) * (1 - i.pos.z + 0.07),0.0,1.0));
            }

            ENDCG            
        }
    }
    FallBack "Diffuse"
}
