#ifdef VERTEX
// #version 150
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shader_bit_encoding : enable

uniform 	vec4 _WorldSpaceLightPos0;
uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	float _Exposure;
uniform 	vec3 _GroundColor;
uniform 	vec3 _SkyTint;
uniform 	float _AtmosphereThickness;
in  vec4 in_POSITION0;
out float vs_TEXCOORD0;
out vec3 vs_TEXCOORD1;
out vec3 vs_TEXCOORD2;
vec4 u_xlat0;
bool u_xlatb0;
vec4 u_xlat1;
vec4 u_xlat2;
vec3 u_xlat3;
vec3 u_xlat4;
vec4 u_xlat5;
vec3 u_xlat6;
float u_xlat7;
vec3 u_xlat9;
float u_xlat14;
vec2 u_xlat16;
float u_xlat21;
float u_xlat22;
float u_xlat23;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat1 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat2 = u_xlat1.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat2 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat1.xxxx + u_xlat2;
    u_xlat2 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat1.zzzz + u_xlat2;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat1.wwww + u_xlat2;
    u_xlat1.xyz = (-_SkyTint.xyz) + vec3(1.0, 1.0, 1.0);
    u_xlat1.xyz = u_xlat1.xyz * vec3(0.300000012, 0.300000042, 0.300000012) + vec3(0.5, 0.419999987, 0.324999988);
    u_xlat1.xyz = u_xlat1.xyz * u_xlat1.xyz;
    u_xlat1.xyz = u_xlat1.xyz * u_xlat1.xyz;
    u_xlat1.xyz = vec3(1.0, 1.0, 1.0) / u_xlat1.xyz;
    u_xlat21 = log2(_AtmosphereThickness);
    u_xlat21 = u_xlat21 * 2.5;
    u_xlat21 = exp2(u_xlat21);
    u_xlat2.xy = vec2(u_xlat21) * vec2(0.049999997, 0.0314159282);
    u_xlat21 = dot(u_xlat0.xyz, u_xlat0.xyz);
    u_xlat21 = inversesqrt(u_xlat21);
    u_xlat3.xyz = vec3(u_xlat21) * u_xlat0.xyz;
    u_xlatb0 = u_xlat3.y>=0.0;
    if(u_xlatb0){
        u_xlat0.x = u_xlat3.y * u_xlat3.y + 0.0506249666;
        u_xlat0.x = sqrt(u_xlat0.x);
        u_xlat0.x = (-u_xlat0.y) * u_xlat21 + u_xlat0.x;
        u_xlat7 = (-u_xlat3.y) * 1.0 + 1.0;
        u_xlat14 = u_xlat7 * 5.25 + -6.80000019;
        u_xlat14 = u_xlat7 * u_xlat14 + 3.82999992;
        u_xlat14 = u_xlat7 * u_xlat14 + 0.458999991;
        u_xlat7 = u_xlat7 * u_xlat14 + -0.00286999997;
        u_xlat7 = u_xlat7 * 1.44269502;
        u_xlat0.y = exp2(u_xlat7);
        u_xlat0.xyz = u_xlat0.xyx * vec3(0.5, 0.246031836, 20.0);
        u_xlat4.xyz = u_xlat0.xxx * u_xlat3.xyz;
        u_xlat4.xyz = u_xlat4.xyz * vec3(0.5, 0.5, 0.5) + vec3(0.0, 1.00010002, 0.0);
        u_xlat21 = dot(u_xlat4.xyz, u_xlat4.xyz);
        u_xlat21 = sqrt(u_xlat21);
        u_xlat22 = (-u_xlat21) + 1.0;
        u_xlat22 = u_xlat22 * 230.831207;
        u_xlat22 = exp2(u_xlat22);
        u_xlat16.x = dot(_WorldSpaceLightPos0.xyz, u_xlat4.xyz);
        u_xlat16.x = u_xlat16.x / u_xlat21;
        u_xlat23 = dot(u_xlat3.xyz, u_xlat4.xyz);
        u_xlat21 = u_xlat23 / u_xlat21;
        u_xlat16.x = (-u_xlat16.x) + 1.0;
        u_xlat23 = u_xlat16.x * 5.25 + -6.80000019;
        u_xlat23 = u_xlat16.x * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat16.x * u_xlat23 + 0.458999991;
        u_xlat16.x = u_xlat16.x * u_xlat23 + -0.00286999997;
        u_xlat16.x = u_xlat16.x * 1.44269502;
        u_xlat16.x = exp2(u_xlat16.x);
        u_xlat21 = (-u_xlat21) + 1.0;
        u_xlat23 = u_xlat21 * 5.25 + -6.80000019;
        u_xlat23 = u_xlat21 * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat21 * u_xlat23 + 0.458999991;
        u_xlat21 = u_xlat21 * u_xlat23 + -0.00286999997;
        u_xlat21 = u_xlat21 * 1.44269502;
        u_xlat21 = exp2(u_xlat21);
        u_xlat21 = u_xlat21 * 0.25;
        u_xlat21 = u_xlat16.x * 0.25 + (-u_xlat21);
        u_xlat21 = u_xlat22 * u_xlat21 + u_xlat0.y;
        u_xlat21 = max(u_xlat21, 0.0);
        u_xlat21 = min(u_xlat21, 50.0);
        u_xlat5.xyz = u_xlat1.xyz * u_xlat2.yyy + vec3(0.0125663709, 0.0125663709, 0.0125663709);
        u_xlat6.xyz = (-vec3(u_xlat21)) * u_xlat5.xyz;
        u_xlat6.xyz = u_xlat6.xyz * vec3(1.44269502, 1.44269502, 1.44269502);
        u_xlat6.xyz = exp2(u_xlat6.xyz);
        u_xlat21 = u_xlat0.z * u_xlat22;
        u_xlat4.xyz = u_xlat3.xyz * u_xlat0.xxx + u_xlat4.xyz;
        u_xlat0.x = dot(u_xlat4.xyz, u_xlat4.xyz);
        u_xlat0.x = sqrt(u_xlat0.x);
        u_xlat22 = (-u_xlat0.x) + 1.0;
        u_xlat22 = u_xlat22 * 230.831207;
        u_xlat22 = exp2(u_xlat22);
        u_xlat16.x = dot(_WorldSpaceLightPos0.xyz, u_xlat4.xyz);
        u_xlat16.x = u_xlat16.x / u_xlat0.x;
        u_xlat23 = dot(u_xlat3.xyz, u_xlat4.xyz);
        u_xlat0.x = u_xlat23 / u_xlat0.x;
        u_xlat16.x = (-u_xlat16.x) + 1.0;
        u_xlat23 = u_xlat16.x * 5.25 + -6.80000019;
        u_xlat23 = u_xlat16.x * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat16.x * u_xlat23 + 0.458999991;
        u_xlat16.x = u_xlat16.x * u_xlat23 + -0.00286999997;
        u_xlat16.x = u_xlat16.x * 1.44269502;
        u_xlat16.x = exp2(u_xlat16.x);
        u_xlat0.x = (-u_xlat0.x) + 1.0;
        u_xlat23 = u_xlat0.x * 5.25 + -6.80000019;
        u_xlat23 = u_xlat0.x * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat0.x * u_xlat23 + 0.458999991;
        u_xlat0.x = u_xlat0.x * u_xlat23 + -0.00286999997;
        u_xlat0.x = u_xlat0.x * 1.44269502;
        u_xlat0.x = exp2(u_xlat0.x);
        u_xlat0.x = u_xlat0.x * 0.25;
        u_xlat0.x = u_xlat16.x * 0.25 + (-u_xlat0.x);
        u_xlat0.x = u_xlat22 * u_xlat0.x + u_xlat0.y;
        u_xlat0.x = max(u_xlat0.x, 0.0);
        u_xlat0.x = min(u_xlat0.x, 50.0);
        u_xlat4.xyz = u_xlat5.xyz * (-u_xlat0.xxx);
        u_xlat4.xyz = u_xlat4.xyz * vec3(1.44269502, 1.44269502, 1.44269502);
        u_xlat4.xyz = exp2(u_xlat4.xyz);
        u_xlat0.x = u_xlat0.z * u_xlat22;
        u_xlat0.xyz = u_xlat0.xxx * u_xlat4.xyz;
        u_xlat0.xyz = u_xlat6.xyz * vec3(u_xlat21) + u_xlat0.xyz;
        u_xlat4.xyz = u_xlat1.xyz * u_xlat2.xxx;
        u_xlat4.xyz = u_xlat0.xyz * u_xlat4.xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.0199999996, 0.0199999996, 0.0199999996);
    } else {
        u_xlat21 = min(u_xlat3.y, -0.00100000005);
        u_xlat21 = -9.99999975e-05 / u_xlat21;
        u_xlat5.xyz = vec3(u_xlat21) * u_xlat3.xyz + vec3(0.0, 1.00010002, 0.0);
        u_xlat22 = dot((-u_xlat3.xyz), u_xlat5.xyz);
        u_xlat16.x = dot(_WorldSpaceLightPos0.xyz, u_xlat5.xyz);
        u_xlat22 = (-u_xlat22) + 1.0;
        u_xlat23 = u_xlat22 * 5.25 + -6.80000019;
        u_xlat23 = u_xlat22 * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat22 * u_xlat23 + 0.458999991;
        u_xlat22 = u_xlat22 * u_xlat23 + -0.00286999997;
        u_xlat22 = u_xlat22 * 1.44269502;
        u_xlat22 = exp2(u_xlat22);
        u_xlat16.x = (-u_xlat16.x) + 1.0;
        u_xlat23 = u_xlat16.x * 5.25 + -6.80000019;
        u_xlat23 = u_xlat16.x * u_xlat23 + 3.82999992;
        u_xlat23 = u_xlat16.x * u_xlat23 + 0.458999991;
        u_xlat16.x = u_xlat16.x * u_xlat23 + -0.00286999997;
        u_xlat16.x = u_xlat16.x * 1.44269502;
        u_xlat16.x = exp2(u_xlat16.x);
        u_xlat5.xy = vec2(u_xlat22) * vec2(0.25, 0.249900013);
        u_xlat22 = u_xlat16.x * 0.25 + u_xlat5.x;
        u_xlat16.xy = vec2(u_xlat21) * vec2(0.5, 20.0);
        u_xlat5.xzw = u_xlat16.xxx * u_xlat3.xyz;
        u_xlat5.xzw = u_xlat5.xzw * vec3(0.5, 0.5, 0.5) + vec3(0.0, 1.00010002, 0.0);
        u_xlat21 = dot(u_xlat5.xzw, u_xlat5.xzw);
        u_xlat21 = sqrt(u_xlat21);
        u_xlat21 = (-u_xlat21) + 1.0;
        u_xlat21 = u_xlat21 * 230.831207;
        u_xlat21 = exp2(u_xlat21);
        u_xlat22 = u_xlat21 * u_xlat22 + (-u_xlat5.y);
        u_xlat22 = max(u_xlat22, 0.0);
        u_xlat22 = min(u_xlat22, 50.0);
        u_xlat5.xyz = u_xlat1.xyz * u_xlat2.yyy + vec3(0.0125663709, 0.0125663709, 0.0125663709);
        u_xlat5.xyz = (-vec3(u_xlat22)) * u_xlat5.xyz;
        u_xlat5.xyz = u_xlat5.xyz * vec3(1.44269502, 1.44269502, 1.44269502);
        u_xlat0.xyz = exp2(u_xlat5.xyz);
        u_xlat21 = u_xlat16.y * u_xlat21;
        u_xlat9.xyz = vec3(u_xlat21) * u_xlat0.xyz;
        u_xlat1.xyz = u_xlat1.xyz * u_xlat2.xxx + vec3(0.0199999996, 0.0199999996, 0.0199999996);
        u_xlat4.xyz = u_xlat1.xyz * u_xlat9.xyz;
    //ENDIF
    }
    vs_TEXCOORD0 = u_xlat3.y * -50.0;
    u_xlat1.xyz = vec3(_GroundColor.x, _GroundColor.y, _GroundColor.z) * vec3(_GroundColor.x, _GroundColor.y, _GroundColor.z);
    u_xlat0.xyz = u_xlat1.xyz * u_xlat0.xyz + u_xlat4.xyz;
    vs_TEXCOORD1.xyz = u_xlat0.xyz * vec3(_Exposure);
    u_xlat0.x = dot(_WorldSpaceLightPos0.xyz, (-u_xlat3.xyz));
    u_xlat0.x = u_xlat0.x * u_xlat0.x;
    u_xlat0.x = u_xlat0.x * 0.75 + 0.75;
    u_xlat0.xyz = u_xlat0.xxx * u_xlat4.xyz;
    vs_TEXCOORD2.xyz = u_xlat0.xyz * vec3(_Exposure);
    return;
}

#endif
#ifdef FRAGMENT
// #version 150
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shader_bit_encoding : enable

in  float vs_TEXCOORD0;
in  vec3 vs_TEXCOORD1;
in  vec3 vs_TEXCOORD2;
layout(location = 0) out vec4 SV_Target0;
vec3 u_xlat0;
vec3 u_xlat1;
void main()
{
    u_xlat0.x = vs_TEXCOORD0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat1.xyz = vs_TEXCOORD1.xyz + (-vs_TEXCOORD2.xyz);
    u_xlat0.xyz = u_xlat0.xxx * u_xlat1.xyz + vs_TEXCOORD2.xyz;
    SV_Target0.xyz = sqrt(u_xlat0.xyz);
    SV_Target0.w = 1.0;
    return;
}

#endif
