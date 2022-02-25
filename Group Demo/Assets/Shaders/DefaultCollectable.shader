Shader "Custom/DefaultCollectable"
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        //_MainTex("Albedo (RGB)", 2D) = "white" {}
        _Glossiness("Smoothness", Range(0,1)) = 0.5
        _Metallic("Metallic", Range(0,1)) = 0.0
        _SpecColor("Specular Color", Color) = (1.0,1.0,1.0,1.0)
        _Shininess("Shininess", Float) = 10
        //_PlayerPos("P", Vector) = (0,0,0,0)
    }
        SubShader{
            Tags{"Queue" = "Transparent" "RenderType" = "Transparent" "IgnoreProjector" = "True"}

            pass {
                Tags{"LightMode" = "ForwardAdd"}

                Cull Back
                ZWrite On
                Blend SrcAlpha OneMinusSrcAlpha

                CGPROGRAM

                #pragma vertex vert
                #pragma fragment frag

                #include "UnityCG.cginc"
                #include "UnityLightingCommon.cginc"

                fixed4 _Color;
                float _Shininess;
                //sampler2D _MainTex;

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

                v2f vert(a2v v) {
                    v2f o;
                    float4 p = v.vertex;// +float4(sin(_Time.z) * v.vertex.x, sin(_Time.z) * v.vertex.y, sin(_Time.z) * v.vertex.z, 1);
                    o.posWorld = mul(unity_ObjectToWorld,p);
                    o.pos = UnityObjectToClipPos(p);
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
                    float atten = 0.2;
                    float3 lightDir = normalize(fragmentToLightSource);
                    float3 diffuseReflection = atten * _LightColor0.xyz * saturate(dot(normal, lightDir));
                    float3 specularReflection = diffuseReflection * _SpecColor.xyz * pow(saturate(dot(reflect(-lightDir, normal), viewDir)), _Shininess);

                    float3 lightFinal = UNITY_LIGHTMODEL_AMBIENT.xyz + diffuseReflection + specularReflection;
                    //float3 wcoord = (i.srcPos.xyz / i.srcPos.w);


                    return  i.color * float4(lightFinal * 5, 1); //(abs(wcoord.x-0.5) + abs(wcoord.y - 0.5)) *clamp(pow(factor, _CullingFactor), 0.0, 1.0)+ 0.0235 ;
                }

                ENDCG
            }
    }
    FallBack "Diffuse"
}
