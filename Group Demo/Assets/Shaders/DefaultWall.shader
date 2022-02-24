// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

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
        _CullingDistance("CullingDistance", Float) = 0.08
        _SpecColor("Specular Color", Color) = (1.0,1.0,1.0,1.0)
        _Shininess("Shininess", Float) = 10
        //_PlayerPos("P", Vector) = (0,0,0,0)
    }
    SubShader{
        Tags{"Queue"="Transparent" "RenderType"="Transparent" "IgnoreProjector"="True"}

        pass {
            Tags{"LightMode"="ForwardAdd"}

            Cull Off
            ZWrite On
            Blend SrcAlpha OneMinusSrcAlpha

            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"

            fixed4 _Color;
            float _Shininess;
            float _CullingFactor;
            float _CullingDistance;
            float4 _PlayerPos;
            //sampler2D _MainTex;
            sampler2D _Tex;
            sampler2D _TexOri;

            struct a2v {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float4 texcoord : TEXCOORD0;
            };

            struct v2f {
                float4 posWorld : TEXCOORD1;
                float4 pos : SV_POSITION;
                float4 texcoord : TEXCOORD0;
                float3 normalDir : TEXCOORD2;
                fixed4 color : COLOR;
            };

            v2f vert(a2v v){
                v2f o;
                o.posWorld = mul(unity_ObjectToWorld, v.vertex);
                o.pos = UnityObjectToClipPos(v.vertex);
                o.normalDir = normalize(mul(float4(v.normal, 0.0), unity_WorldToObject).xyz);
                o.texcoord = v.texcoord;
                o.color = _Color;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                float3 normal = i.normalDir;
                float3 viewDir = normalize(_WorldSpaceCameraPos.xyz - i.pos.xyz);
                float3 fragmentToLightSource = _WorldSpaceLightPos0.xyz - i.posWorld.xyz;
                float distance = length(fragmentToLightSource);
                float atten = 1;
                float3 lightDir = normalize(fragmentToLightSource);
                float3 diffuseReflection = atten * _LightColor0.xyz * saturate(dot(normal, lightDir));
                float3 specularReflection = diffuseReflection * _SpecColor.xyz * pow(saturate(dot(reflect(-lightDir, normal), viewDir)), _Shininess);

                float3 lightFinal = UNITY_LIGHTMODEL_AMBIENT.xyz + diffuseReflection + specularReflection;
                //float3 wcoord = (i.srcPos.xyz / i.srcPos.w);
                float4 playPos = mul(UNITY_MATRIX_P,mul(UNITY_MATRIX_V,_PlayerPos));
                
                fixed4 color = tex2D(_Tex,i.texcoord);
                if (color.a > 0.7) {
                    i.color = tex2D(_TexOri, i.texcoord);
                    i.color.a = 1;
                }
                if(i.pos.z + 0.0235 < playPos.z) return i.color * float4(lightFinal, 1); //(1 - i.pos.z + 0.07) playPos.z < 0.0298
                float factor = 1.0 - (i.pos.z + 0.0235 - playPos.z);
                return  i.color * float4(lightFinal, 0.3); //(abs(wcoord.x-0.5) + abs(wcoord.y - 0.5)) *clamp(pow(factor, _CullingFactor), 0.0, 1.0)+ 0.0235 ;
            }

            ENDCG            
        }
    }
    FallBack "Diffuse"
}
