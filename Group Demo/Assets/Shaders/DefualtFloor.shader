// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'

Shader "Custom/DefualtFloor"
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        //_MainTex("Albedo (RGB)", 2D) = "white" {}
        _Tex("Tex (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
        _SpecColor("Specular Color", Color) = (1.0,1.0,1.0,1.0)
        _Shininess("Shininess", Float) = 10
    }
    SubShader{
        Tags{"Queue" = "Geometry"}
        pass {
            Tags{"LightMode" = "ForwardAdd"}

            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"
            fixed4 _Color;
            float _Shininess;
            //sampler2D _MainTex;
            sampler2D _Tex;
            sampler2D _TexOri;

            struct a2v {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float4 texcoord : TEXCOORD0;
            };

            struct v2f {
                float4 vert : TEXCOORD3;
                float4 posWorld : TEXCOORD1;
                float4 pos : SV_POSITION;
                float4 texcoord : TEXCOORD0;
                float3 normalDir : TEXCOORD2;
                fixed4 color : COLOR;
            };

            v2f vert(a2v v) {
                v2f o;
                o.vert = v.vertex;
                o.posWorld = mul(unity_ObjectToWorld, v.vertex);
                o.pos = UnityObjectToClipPos(v.vertex);
                o.normalDir = normalize(mul(float4(v.normal, 0.0), unity_WorldToObject).xyz);
                o.texcoord = v.texcoord;
                o.color = _Color;
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                //float3 wcoord = (i.srcPos.xyz / i.srcPos.w);
                float3 normal = i.normalDir + float3(0.1,0.1,0.1) * sin(_Time.y * 2 + sin(i.vert.z) * 6) * sin(_Time.z + sin(i.vert.x) * 14) * sin(_Time.w + sin(i.vert.y) * 20);
                normal = normalize(normal);
                float3 viewDir = normalize(_WorldSpaceCameraPos.xyz - i.pos.xyz);
                float3 fragmentToLightSource = _WorldSpaceLightPos0.xyz - i.posWorld.xyz;
                float distance = length(fragmentToLightSource);
                float atten = 0.7;
                float3 lightDir = normalize(fragmentToLightSource);
                float3 diffuseReflection = atten * _LightColor0.xyz * saturate(dot(normal, lightDir));
                float3 specularReflection = diffuseReflection * _SpecColor.xyz * pow(saturate(dot(reflect(-lightDir, normal), viewDir)), _Shininess);

                float3 lightFinal = UNITY_LIGHTMODEL_AMBIENT.xyz + diffuseReflection + specularReflection;

                fixed4 color = tex2D(_Tex,i.texcoord);
                if (color.a > 0.7) {
                    i.color = tex2D(_TexOri, i.texcoord) * max(color.z, max(color.x, color.y));
                    i.color.a = 1;
                }
                return i.color * float4(lightFinal,1); //(1 - i.pos.z + 0.07)
            }

            ENDCG
        }
    }
    FallBack "Diffuse"
}
