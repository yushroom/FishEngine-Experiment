


struct V2F
{
    vec2 UV;
    vec3 ScreenVector;
};

#ifdef VERTEX
    // Unity built-in shader source. Copyright (c) 2016 Unity Technologies. MIT license (see license.txt)

#ifndef CG_inc
#define CG_inc

#ifndef ShaderVariables_inc
#define ShaderVariables_inc

#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

#define CBUFFER_START(name) layout(std140, row_major) uniform name {
#define CBUFFER_END };

layout(std140, row_major) uniform PerCameraUniforms
{
    mat4 MATRIX_P;          // CameraProjection;
    mat4 MATRIX_V;          // WorldToCamera;
    mat4 MATRIX_I_V;        // CameraToWorld;
    mat4 MATRIX_VP;         // CameraProjection * WorldToCamera;

    // Time (t = time since current level load) values from Unity
    vec4 Time;  // (t/20, t, t*2, t*3)
    //vec4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
    //vec4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
    //vec4 unity_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

    // x = 1 or -1 (-1 if projection is flipped)
    // y = near plane
    // z = far plane
    // w = 1/far plane
    vec4 ProjectionParams;
    
    // x = width
    // y = height
    // z = 1 + 1.0/width
    // w = 1 + 1.0/height
    vec4 ScreenParams;
    
    // Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
    // x = 1-far/near
    // y = far/near
    // z = x/far
    // w = y/far
    vec4 ZBufferParams;

    // x = orthographic camera's width
    // y = orthographic camera's height
    // z = unused
    // w = 1.0 if camera is ortho, 0.0 if perspective
    //vec4 unity_OrthoParams;
    
    vec4 WorldSpaceCameraPos;       // .w = 1, not used
    vec4 WorldSpaceCameraDir;       // .w = 0, not used, forward direction of the camera in world space
};


layout(std140, row_major) uniform PerDrawUniforms
{
    mat4 MATRIX_MVP;
    mat4 MATRIX_MV;
    mat4 MATRIX_IT_MV;
    mat4 MATRIX_M;      // ObjectToWorld
    mat4 MATRIX_IT_M;   // WorldToObject
};

// layout(std140, row_major) uniform PerFrameUniforms
// {
// };

layout(std140, row_major) uniform LightingUniforms
{
    vec4 LightColor;
    vec4 WorldSpaceLightPos;

    vec4 CascadesNear;
    vec4 CascadesFar;
    vec4 CascadesSplitPlaneNear;
    vec4 CascadesSplitPlaneFar;
    vec4 _LightShadowData;
    vec4 unity_LightShadowBias;
    //mat4 LightMatrix; // World-to-light matrix. Used to sample cookie & attenuation textures.
    // macOS bug
    layout(column_major) mat4 LightMatrix[4]; // world -> clip (VP)
};

// CBUFFER_START(UnityShadows)
//  //float4 unity_ShadowSplitSpheres[4];
//  //float4 unity_ShadowSplitSqRadii;
//  float4 unity_LightShadowBias;
//  float4 _LightSplitsNear;
//  float4 _LightSplitsFar;
//  float4x4 unity_WorldToShadow[4];
//  half4 _LightShadowData;
//  float4 unity_ShadowFadeCenterAndType;
// CBUFFER_END



#define MAX_BONE_SIZE 128
// column_major: macOS bug
layout(std140, column_major) uniform Bones
{
    mat4 BoneTransformations[MAX_BONE_SIZE];
};


#endif // ShaderVariables_inc


#ifndef UnitySupport_inc
#define UnitySupport_inc

#ifndef CGSupport_inc
#define CGSupport_inc

#define PI 3.141592653589793
#define INV_PI 0.3183098861837907

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4

#define half float
#define half2 vec2
#define half3 vec3
#define half4 vec4

#define samplerCUBE samplerCube

// http://http.developer.nvidia.com/Cg/lit.html
float4 lit(float NdotL, float NdotH, float m)
{
    float specular = (NdotL > 0) ? pow(max(0.0, NdotH), m) : 0;
    return float4(1.0, max(0.0, NdotL), specular, 1.0);
}

void sincos(float a, out float s, out float c)
{
    s = sin(a);
    c = cos(a);
}

// http://http.developer.nvidia.com/Cg/lerp.html
//#define lerp(x, y, t) mix(x, y, t)
float lerp(float a, float b, float w)
{
    return a + w*(b-a);
}

float2 lerp(float2 a, float2 b, float2 w)
{
    return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float3 w)
{
    return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float4 w)
{
    return a + w*(b-a);
}


float2 lerp(float2 a, float2 b, float w)
{
    return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float w)
{
    return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float w)
{
    return a + w*(b-a);
}

// http://http.developer.nvidia.com/Cg/saturate.html
#define saturate(x) clamp((x), 0.0, 1.0)

//http://http.developer.nvidia.com/Cg/frac.html
#define frac(x) fract(x)

// http://http.developer.nvidia.com/Cg/tex2D.html
#define tex2D texture
#define texCUBE texture

#define mul(M, V) ((M) * (V))

#endif //CGSupport_inc


#ifndef ShaderVariables_inc
#define ShaderVariables_inc

#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

#define CBUFFER_START(name) layout(std140, row_major) uniform name {
#define CBUFFER_END };

layout(std140, row_major) uniform PerCameraUniforms
{
    mat4 MATRIX_P;          // CameraProjection;
    mat4 MATRIX_V;          // WorldToCamera;
    mat4 MATRIX_I_V;        // CameraToWorld;
    mat4 MATRIX_VP;         // CameraProjection * WorldToCamera;

    // Time (t = time since current level load) values from Unity
    vec4 Time;  // (t/20, t, t*2, t*3)
    //vec4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
    //vec4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
    //vec4 unity_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

    // x = 1 or -1 (-1 if projection is flipped)
    // y = near plane
    // z = far plane
    // w = 1/far plane
    vec4 ProjectionParams;
    
    // x = width
    // y = height
    // z = 1 + 1.0/width
    // w = 1 + 1.0/height
    vec4 ScreenParams;
    
    // Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
    // x = 1-far/near
    // y = far/near
    // z = x/far
    // w = y/far
    vec4 ZBufferParams;

    // x = orthographic camera's width
    // y = orthographic camera's height
    // z = unused
    // w = 1.0 if camera is ortho, 0.0 if perspective
    //vec4 unity_OrthoParams;
    
    vec4 WorldSpaceCameraPos;       // .w = 1, not used
    vec4 WorldSpaceCameraDir;       // .w = 0, not used, forward direction of the camera in world space
};


layout(std140, row_major) uniform PerDrawUniforms
{
    mat4 MATRIX_MVP;
    mat4 MATRIX_MV;
    mat4 MATRIX_IT_MV;
    mat4 MATRIX_M;      // ObjectToWorld
    mat4 MATRIX_IT_M;   // WorldToObject
};

// layout(std140, row_major) uniform PerFrameUniforms
// {
// };

layout(std140, row_major) uniform LightingUniforms
{
    vec4 LightColor;
    vec4 WorldSpaceLightPos;

    vec4 CascadesNear;
    vec4 CascadesFar;
    vec4 CascadesSplitPlaneNear;
    vec4 CascadesSplitPlaneFar;
    vec4 _LightShadowData;
    vec4 unity_LightShadowBias;
    //mat4 LightMatrix; // World-to-light matrix. Used to sample cookie & attenuation textures.
    // macOS bug
    layout(column_major) mat4 LightMatrix[4]; // world -> clip (VP)
};

// CBUFFER_START(UnityShadows)
//  //float4 unity_ShadowSplitSpheres[4];
//  //float4 unity_ShadowSplitSqRadii;
//  float4 unity_LightShadowBias;
//  float4 _LightSplitsNear;
//  float4 _LightSplitsFar;
//  float4x4 unity_WorldToShadow[4];
//  half4 _LightShadowData;
//  float4 unity_ShadowFadeCenterAndType;
// CBUFFER_END



#define MAX_BONE_SIZE 128
// column_major: macOS bug
layout(std140, column_major) uniform Bones
{
    mat4 BoneTransformations[MAX_BONE_SIZE];
};


#endif // ShaderVariables_inc



#define UNITY_PI    3.14159265359f

// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
#define glstate_matrix_mvp MATRIX_MVP

// #define UNITY_MATRIX_P glstate_matrix_projection
// #define UNITY_MATRIX_V unity_MatrixV
// #define UNITY_MATRIX_I_V unity_MatrixInvV
// #define UNITY_MATRIX_VP unity_MatrixVP
// #define UNITY_MATRIX_M unity_ObjectToWorld
// #define UNITY_MATRIX_MVP glstate_matrix_mvp
// #define UNITY_MATRIX_MV glstate_matrix_modelview0
// #define UNITY_MATRIX_T_MV glstate_matrix_transpose_modelview0
// #define UNITY_MATRIX_IT_MV glstate_matrix_invtrans_modelview0
#define UNITY_MATRIX_MVP MATRIX_MVP
#define Object2World MATRIX_M
#define unity_ObjectToWorld Object2World
#define unity_WorldToObject MATRIX_IT_M
#define unity_MatrixVP MATRIX_VP

// UnituShaderVariables.cginc
#define UNITY_MATRIX_P glstate_matrix_projection
#define UNITY_MATRIX_V unity_MatrixV
#define UNITY_MATRIX_I_V unity_MatrixInvV
#define UNITY_MATRIX_VP unity_MatrixVP
#define UNITY_MATRIX_M unity_ObjectToWorld

#define _WorldSpaceCameraPos WorldSpaceCameraPos

#define _WorldSpaceLightPos0 WorldSpaceLightPos
#define _LightColor0 LightColor

#define WorldSpaceLightPos0 WorldSpaceLightPos
#define LightColor0 LightColor


// no keyword inline in glsl
#define inline

// #if defined(UNITY_REVERSED_Z)
// #define UNITY_NEAR_CLIP_VALUE (1.0)
// #elif defined(SHADER_API_D3D9)  || defined(SHADER_API_WIIU) || defined(SHADER_API_D3D11_9X)
// #define UNITY_NEAR_CLIP_VALUE (0.0)
// #else
// #define UNITY_NEAR_CLIP_VALUE (-1.0)
// #endif


#endif // UnitySupport_inc




// Tranforms position from object to homogenous space
float4 UnityObjectToClipPos( in float3 pos )
{
    return mul(UNITY_MATRIX_MVP, float4(pos, 1.0));
}

float4 UnityObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return UnityObjectToClipPos(pos.xyz);
}

vec4 ObjectToViewPos( in vec3 pos )
{
    return MATRIX_MV * vec4(pos, 1.0);
}

vec4 ObjectToViewPos( in vec4 pos )
{
    return ObjectToViewPos( pos.xyz );
}

vec4 ObjectToClipPos( in vec3 pos )
{
    return MATRIX_MVP * vec4(pos, 1.0);
}

vec4 ObjectToClipPos( vec4 pos )
{
    return ObjectToClipPos(pos.xyz);
}

vec4 WorldToClipPos( in vec3 pos)
{
    return MATRIX_VP * vec4(pos, 1.0);
}

vec4 ViewToClipPos( in vec3 pos )
{
    return MATRIX_P * vec4(pos, 1.0);
}

vec3 WorldSpaceLightDir( in vec3 worldPos )
{
    return WorldSpaceLightPos.xyz - worldPos * WorldSpaceLightPos.w;
}

vec3 WorldSpaceViewDir( in vec3 worldPos )
{
    return WorldSpaceCameraPos.xyz - worldPos;
}


// Transforms normal from object to world space
inline float3 UnityObjectToWorldNormal( in float3 norm )
{
#ifdef UNITY_ASSUME_UNIFORM_SCALING
    return UnityObjectToWorldDir(norm);
#else
    // mul(IT_M, norm) => mul(norm, I_M) => {dot(norm, I_M.col0), dot(norm, I_M.col1), dot(norm, I_M.col2)}
    return normalize(mul(norm, float3x3(unity_WorldToObject)));
#endif
}

// Computes world space light direction, from world space position
inline float3 UnityWorldSpaceLightDir( in float3 worldPos )
{
    #ifndef USING_LIGHT_MULTI_COMPILE
        return _WorldSpaceLightPos0.xyz - worldPos * _WorldSpaceLightPos0.w;
    #else
        #ifndef USING_DIRECTIONAL_LIGHT
        return _WorldSpaceLightPos0.xyz - worldPos;
        #else
        return _WorldSpaceLightPos0.xyz;
        #endif
    #endif
}


/************************************************************************/
/* Shadow                                                               */
/************************************************************************/

float4 UnityClipSpaceShadowCasterPos(float4 vertex, float3 normal)
{
    float4 wPos = mul(unity_ObjectToWorld, vertex);

    if (unity_LightShadowBias.z != 0.0)
    {
        float3 wNormal = UnityObjectToWorldNormal(normal);
        float3 wLight = normalize(UnityWorldSpaceLightDir(wPos.xyz));

        // apply normal offset bias (inset position along the normal)
        // bias needs to be scaled by sine between normal and light direction
        // (http://the-witness.net/news/2013/09/shadow-mapping-summary-part-1/)
        //
        // unity_LightShadowBias.z contains user-specified normal offset amount
        // scaled by world space texel size.

        float shadowCos = dot(wNormal, wLight);
        float shadowSine = sqrt(1-shadowCos*shadowCos);
        float normalBias = unity_LightShadowBias.z * shadowSine;

        wPos.xyz -= wNormal * normalBias;
    }

    return mul(UNITY_MATRIX_VP, wPos);
}

#define UNITY_NEAR_CLIP_VALUE -1.0

float4 UnityApplyLinearShadowBias(float4 clipPos)
{
#if defined(UNITY_REVERSED_Z)
    // We use max/min instead of clamp to ensure proper handling of the rare case
    // where both numerator and denominator are zero and the fraction becomes NaN.
    clipPos.z += max(-1, min(unity_LightShadowBias.x / clipPos.w, 0));
    float clamped = min(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
#else
    clipPos.z += saturate(unity_LightShadowBias.x/clipPos.w);
    float clamped = max(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
#endif
    clipPos.z = lerp(clipPos.z, clamped, unity_LightShadowBias.y);
    return clipPos;
}


#ifdef SHADOWS_CUBE
    // Rendering into point light (cubemap) shadows
    #define V2F_SHADOW_CASTER_NOPOS float3 vec : TEXCOORD0;
    #define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
    #define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
    #define SHADOW_CASTER_FRAGMENT(i) return UnityEncodeCubeShadowDepth ((length(i.vec) + unity_LightShadowBias.x) * _LightPositionRange.w);
#else
    // Rendering into directional or spot light shadows
    #define V2F_SHADOW_CASTER_NOPOS
    // Let embedding code know that V2F_SHADOW_CASTER_NOPOS is empty; so that it can workaround
    // empty structs that could possibly be produced.
    #define V2F_SHADOW_CASTER_NOPOS_IS_EMPTY
    #define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) \
        opos = UnityObjectToClipPos(v.vertex.xyz); \
        opos = UnityApplyLinearShadowBias(opos);
    #define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) \
        opos = UnityClipSpaceShadowCasterPos(v.vertex, v.normal); \
        opos = UnityApplyLinearShadowBias(opos);
    #define SHADOW_CASTER_FRAGMENT(i) return 0;
#endif

// Declare all data needed for shadow caster pass output (any shadow directions/depths/distances as needed),
// plus clip space position.
#define V2F_SHADOW_CASTER V2F_SHADOW_CASTER_NOPOS float4 pos : SV_POSITION

// Vertex shader part, with support for normal offset shadows. Requires
// position and normal to be present in the vertex input.
#define TRANSFER_SHADOW_CASTER_NORMALOFFSET(o) TRANSFER_SHADOW_CASTER_NOPOS(o,o.pos)


#endif /* CG_inc */




    layout (location = PositionIndex) in vec3 InputPosition;
    layout (location = UVIndex) in vec2 InputUV;

    out V2F v2f;

    void main()
    {
        gl_Position = vec4(InputPosition.x, InputPosition.y, 0.f, 1.f);
        //gl_Position = ObjectToClipPos(InputPosition);
        // Screen to world
        v2f.UV = InputUV * vec2(1, -1); // OpenGL
        
        vec4 WorldPosition = inverse(MATRIX_VP) * vec4(InputPosition.x, InputPosition.y, 1.0f, 1.0f);
        // in world space
        v2f.ScreenVector = normalize(WorldPosition.xyz / WorldPosition.w - WorldSpaceCameraPos.xyz);
    }
#endif /*VERTEX_SHADER*/


#ifdef FRAGMENT
    #ifndef Common_inc
#define Common_inc

#ifndef ShaderVariables_inc
#define ShaderVariables_inc

#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

#define CBUFFER_START(name) layout(std140, row_major) uniform name {
#define CBUFFER_END };

layout(std140, row_major) uniform PerCameraUniforms
{
    mat4 MATRIX_P;          // CameraProjection;
    mat4 MATRIX_V;          // WorldToCamera;
    mat4 MATRIX_I_V;        // CameraToWorld;
    mat4 MATRIX_VP;         // CameraProjection * WorldToCamera;

    // Time (t = time since current level load) values from Unity
    vec4 Time;  // (t/20, t, t*2, t*3)
    //vec4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
    //vec4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
    //vec4 unity_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

    // x = 1 or -1 (-1 if projection is flipped)
    // y = near plane
    // z = far plane
    // w = 1/far plane
    vec4 ProjectionParams;
    
    // x = width
    // y = height
    // z = 1 + 1.0/width
    // w = 1 + 1.0/height
    vec4 ScreenParams;
    
    // Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
    // x = 1-far/near
    // y = far/near
    // z = x/far
    // w = y/far
    vec4 ZBufferParams;

    // x = orthographic camera's width
    // y = orthographic camera's height
    // z = unused
    // w = 1.0 if camera is ortho, 0.0 if perspective
    //vec4 unity_OrthoParams;
    
    vec4 WorldSpaceCameraPos;       // .w = 1, not used
    vec4 WorldSpaceCameraDir;       // .w = 0, not used, forward direction of the camera in world space
};


layout(std140, row_major) uniform PerDrawUniforms
{
    mat4 MATRIX_MVP;
    mat4 MATRIX_MV;
    mat4 MATRIX_IT_MV;
    mat4 MATRIX_M;      // ObjectToWorld
    mat4 MATRIX_IT_M;   // WorldToObject
};

// layout(std140, row_major) uniform PerFrameUniforms
// {
// };

layout(std140, row_major) uniform LightingUniforms
{
    vec4 LightColor;
    vec4 WorldSpaceLightPos;

    vec4 CascadesNear;
    vec4 CascadesFar;
    vec4 CascadesSplitPlaneNear;
    vec4 CascadesSplitPlaneFar;
    vec4 _LightShadowData;
    vec4 unity_LightShadowBias;
    //mat4 LightMatrix; // World-to-light matrix. Used to sample cookie & attenuation textures.
    // macOS bug
    layout(column_major) mat4 LightMatrix[4]; // world -> clip (VP)
};

// CBUFFER_START(UnityShadows)
//  //float4 unity_ShadowSplitSpheres[4];
//  //float4 unity_ShadowSplitSqRadii;
//  float4 unity_LightShadowBias;
//  float4 _LightSplitsNear;
//  float4 _LightSplitsFar;
//  float4x4 unity_WorldToShadow[4];
//  half4 _LightShadowData;
//  float4 unity_ShadowFadeCenterAndType;
// CBUFFER_END



#define MAX_BONE_SIZE 128
// column_major: macOS bug
layout(std140, column_major) uniform Bones
{
    mat4 BoneTransformations[MAX_BONE_SIZE];
};


#endif // ShaderVariables_inc



// (x, y) bottom left corner position (in clip space, (-1, 1))
// (z, w) width and height, in range [0, 1]
// eg. (-1, -1, 1, 1) for full sreen quad
uniform vec4 DrawRectParameters;

// Screen Aligned Quad
vec4 DrawRect(in vec2 InPosition)
{
    return vec4( (InPosition - DrawRectParameters.xy) * DrawRectParameters.zw + DrawRectParameters.xy ,0, 1 );
}

uniform sampler2D SceneDepthTexture;

// also see ConvertToDeviceZ()
// @param DeviceZ value that is stored in the depth buffer (Z/W)
// @return SceneDepth (linear in world units, W)
// float ConvertFromDeviceZ(float DeviceZ)
// {
//  // Supports ortho and perspective, see CreateInvDeviceZToWorldZTransform()
//  return DeviceZ * View.InvDeviceZToWorldZTransform[0] + View.InvDeviceZToWorldZTransform[1] + 1.0f / (DeviceZ * View.InvDeviceZToWorldZTransform[2] - View.InvDeviceZToWorldZTransform[3]);
// }


// Z buffer to linear 0..1 depth (0 at eye, 1 at far plane)
float Linear01Depth( float z )
{
    return 1.0 / (ZBufferParams.x * z + ZBufferParams.y);
}
// Z buffer to linear depth
float LinearEyeDepth( float z )
{
    //(n * f) / (f - z * (f - n))
    return 1.0 / (ZBufferParams.z * z + ZBufferParams.w);
}

// Returns clip space W, which is world space distance along the View Z axis.
float CalcSceneDepth( vec2 ScreenUV )
{
    // (2 * n * f) / (f + n - z * (f - n))
    //return LinearEyeDepth( textureLod( SceneDepthTexture, ScreenUV, 0 ).r );
    float DeviceZ = textureLod( SceneDepthTexture, ScreenUV, 0 ).r;
    return LinearEyeDepth(DeviceZ);
}

#endif


    #ifndef CascadedShadowMapCommon_inc
#define CascadedShadowMapCommon_inc

#ifndef CGSupport_inc
#define CGSupport_inc

#define PI 3.141592653589793
#define INV_PI 0.3183098861837907

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4

#define half float
#define half2 vec2
#define half3 vec3
#define half4 vec4

#define samplerCUBE samplerCube

// http://http.developer.nvidia.com/Cg/lit.html
float4 lit(float NdotL, float NdotH, float m)
{
    float specular = (NdotL > 0) ? pow(max(0.0, NdotH), m) : 0;
    return float4(1.0, max(0.0, NdotL), specular, 1.0);
}

void sincos(float a, out float s, out float c)
{
    s = sin(a);
    c = cos(a);
}

// http://http.developer.nvidia.com/Cg/lerp.html
//#define lerp(x, y, t) mix(x, y, t)
float lerp(float a, float b, float w)
{
    return a + w*(b-a);
}

float2 lerp(float2 a, float2 b, float2 w)
{
    return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float3 w)
{
    return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float4 w)
{
    return a + w*(b-a);
}


float2 lerp(float2 a, float2 b, float w)
{
    return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float w)
{
    return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float w)
{
    return a + w*(b-a);
}

// http://http.developer.nvidia.com/Cg/saturate.html
#define saturate(x) clamp((x), 0.0, 1.0)

//http://http.developer.nvidia.com/Cg/frac.html
#define frac(x) fract(x)

// http://http.developer.nvidia.com/Cg/tex2D.html
#define tex2D texture
#define texCUBE texture

#define mul(M, V) ((M) * (V))

#endif //CGSupport_inc



//const float ExpC = 120.0;
//const float bias = 0.05;

uniform sampler2DArrayShadow CascadedShadowMap;

// vec4(1.0 / width, 1.0 / height, width, height)
// eg. (0.00048828125, 0.00048828125, 2048, 2048)
uniform vec4 _ShadowMapTexture_TexelSize;

vec4 GetCascadeWeights( float Depth )
{
    vec4 near = step( CascadesSplitPlaneNear, vec4(Depth) );
    vec4 far  = step( Depth, CascadesSplitPlaneFar);
    return near * far;
}

vec4 GetShadowCoord( vec4 WorldPosition, vec4 CascadeWeights )
{
    vec3 sc0 = (LightMatrix[0] * WorldPosition).xyz;
    vec3 sc1 = (LightMatrix[1] * WorldPosition).xyz;
    vec3 sc2 = (LightMatrix[2] * WorldPosition).xyz;
    vec3 sc3 = (LightMatrix[3] * WorldPosition).xyz;
    vec4 shadowMapCoordinate = vec4(sc0*CascadeWeights[0] + sc1*CascadeWeights[1] + sc2*CascadeWeights[2] + sc3*CascadeWeights[3], 1);
    return shadowMapCoordinate;
}


/**
 * PCF shadowmap filtering based on a 5x5 kernel (optimized with 9 taps)
 *
 * Algorithm: http://the-witness.net/news/2013/09/shadow-mapping-summary-part-1/
 * Implementation example: http://mynameismjp.wordpress.com/2013/09/10/shadow-maps/
 */
float SampleCascadeShadowMap_PCF5x5( vec3 Coord, float Section )
{
    const float2 offset = float2(0.5,0.5);
    float2 uv = (Coord.xy * _ShadowMapTexture_TexelSize.zw) + offset;
    float2 base_uv = (floor(uv) - offset) * _ShadowMapTexture_TexelSize.xy;
    float2 st = frac(uv);

    float3 uw = float3( 4-3*st.x, 7, 1+3*st.x );
    float3 u = float3( (3-2*st.x) / uw.x - 2, (3+st.x)/uw.y, st.x/uw.z + 2 );
    u *= _ShadowMapTexture_TexelSize.x;

    float3 vw = float3( 4-3*st.y, 7, 1+3*st.y );
    float3 v = float3( (3-2*st.y) / vw.x - 2, (3+st.y)/vw.y, st.y/vw.z + 2 );
    v *= _ShadowMapTexture_TexelSize.y;

    half shadow;
    half sum = 0.0f;

    //float bias = - unity_LightShadowBias.x * LightMatrix[int(Section)][2][2];
    //float z = Coord.z - bias;
    float z = Coord.z;

    half3 accum = uw * vw.x;
    sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.x), Section, z ));
    sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.x), Section, z ));
    sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.x), Section, z ));

    accum = uw * vw.y;
    sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.y), Section, z ));
    sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.y), Section, z ));
    sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.y), Section, z ));

    accum = uw * vw.z;
    sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.z), Section, z ));
    sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.z), Section, z ));
    sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.z), Section, z ));

    shadow = sum / 144.0f;
    return shadow;
}

half UnityComputeShadowFade(float fadeDist)
{
    return saturate(fadeDist * _LightShadowData.z + _LightShadowData.w);
}

// @param WorldPosition position in world space
// @param Depth depth to camera
float CalcShadowTerm(vec4 WorldPosition, float Depth)
{
    vec4 CascadeWeights = GetCascadeWeights( Depth );
    float fSection = dot(vec4(0, 1, 2, 3), CascadeWeights);

    //uint Section = uint(fSection);
    //vec4 ProjCoords = LightMatrix[Section] * WorldPosition;
    vec4 ProjCoords = GetShadowCoord( WorldPosition, CascadeWeights );
    ProjCoords.xyz = ProjCoords.xyz * (0.5 / ProjCoords.w) + 0.5;
    float Z = ProjCoords.z;
    float Shadow = SampleCascadeShadowMap_PCF5x5(ProjCoords.xyz, fSection);
    //Shadow += saturate(Z * 0.03333 - 2.7); // from unity
    Shadow += UnityComputeShadowFade(Z);
    // return shadow;
    return saturate(Shadow);
}


float CalcShadowTerm(vec4 WorldPosition)
{
    float Depth = distance(WorldPosition.xyz, WorldSpaceCameraPos.xyz);
    return CalcShadowTerm(WorldPosition, Depth);
}

#endif

    //#include <ShadowCommon.inc>

    in V2F v2f;
    out float OutShadow;


    void main()
    {
        vec3 CameraVector = normalize(v2f.ScreenVector);
        float SceneDepth = CalcSceneDepth(v2f.UV);
        float z = dot(CameraVector, WorldSpaceCameraDir.xyz);
        vec3 WorldPosition = CameraVector * (SceneDepth / z) + WorldSpaceCameraPos.xyz;
        OutShadow = CalcShadowTerm(vec4(WorldPosition, 1), SceneDepth);
    }
#endif /*FRAGMENT_SHADER*/