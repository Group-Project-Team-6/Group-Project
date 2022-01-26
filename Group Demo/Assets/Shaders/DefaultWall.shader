// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/DefaultWall" {
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        _MainTex("Albedo (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
        _PlayerPos("PlayerPos", Vector) = (0,0,0,0)
        _IsCasted("IsCasted", Float) = 1.0
    }
    SubShader
    {
        Tags { "RenderType" = "Fade" }
        LOD 200
        CGPROGRAM
        struct appdata
        {
            float4 vertex : POSITION;
            float2 uv : TEXCOORD0;
        };
        struct v2f
        {
            float2 uv : TEXCOORD0;
            float4 vertex : SV_POSITION;
            float4 vertPlayer : SV_POSITION;
        };

        sampler2D _MainTex;
        fixed4 _Colour;
        half _Glossiness;
        half _Metallic;
        float4 _PlayerPos;
        float  _IsCasted;

        #pragma vertex vert             
        #pragma fragment frag

        v2f vert(appdata v)
        {
            v2f o;
            o.vertex = UnityObjectToClipPos(v.vertex);
            o.vertPlayer = UnityObjectToClipPos(_PlayerPos);
            o.uv = v.uv;
            return o;
        }

        fixed4 frag(v2f i) : COLOR
        {
            // sample the texture
            fixed4 col = tex2D(_MainTex, i.uv);
                // float l = distance(i.vertUnclip, i.vertPlayer)
            //col.w = 0.3;
            return col;
        }
        ENDCG

    }
    FallBack "Diffuse"
}
