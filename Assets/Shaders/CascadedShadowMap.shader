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
	mat4 MATRIX_P;			 
	mat4 MATRIX_V;			 
	mat4 MATRIX_I_V;		 
	mat4 MATRIX_VP;			 

	 
	vec4 Time; 	 
	 
	 
	 

	 
	 
	 
	 
	vec4 ProjectionParams;
	
	 
	 
	 
	 
	vec4 ScreenParams;
	
	 
	 
	 
	 
	 
	vec4 ZBufferParams;

	 
	 
	 
	 
	 
	
	vec4 WorldSpaceCameraPos;		 
	vec4 WorldSpaceCameraDir;		 
};


layout(std140, row_major) uniform PerDrawUniforms
{
	mat4 MATRIX_MVP;
	mat4 MATRIX_MV;
	mat4 MATRIX_IT_MV;
	mat4 MATRIX_M;		 
	mat4 MATRIX_IT_M;	 
};

 
 
 

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
	 
	 
	layout(column_major) mat4 LightMatrix[4];  
};

 
 
 
 
 
 
 
 
 
 



#define MAX_BONE_SIZE 128
 
layout(std140, column_major) uniform Bones
{
	mat4 BoneTransformations[MAX_BONE_SIZE];
};


#endif  

 

#ifndef CG_inc
#define CG_inc



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

 
#define saturate(x) clamp((x), 0.0, 1.0)

 
#define frac(x) fract(x)

 
#define tex2D texture
#define texCUBE texture

#define mul(M, V) ((M) * (V))

#endif  




#define UNITY_PI	3.14159265359f

 
#define glstate_matrix_mvp MATRIX_MVP

 
 
 
 
 
 
 
 
 
#define UNITY_MATRIX_MVP MATRIX_MVP
#define Object2World MATRIX_M
#define unity_ObjectToWorld Object2World
#define unity_WorldToObject MATRIX_IT_M
#define unity_MatrixVP MATRIX_VP

 
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


 
#define inline

 
 
 
 
 
 
 


#endif  



 
float4 UnityObjectToClipPos( in float3 pos )
{
	return mul(UNITY_MATRIX_MVP, float4(pos, 1.0));
}

float4 UnityObjectToClipPos(float4 pos)  
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


 
inline float3 UnityObjectToWorldNormal( in float3 norm )
{
#ifdef UNITY_ASSUME_UNIFORM_SCALING
	return UnityObjectToWorldDir(norm);
#else
	 
	return normalize(mul(norm, float3x3(unity_WorldToObject)));
#endif
}

 
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


 
 
 

float4 UnityClipSpaceShadowCasterPos(float4 vertex, float3 normal)
{
	float4 wPos = mul(unity_ObjectToWorld, vertex);

	if (unity_LightShadowBias.z != 0.0)
	{
		float3 wNormal = UnityObjectToWorldNormal(normal);
		float3 wLight = normalize(UnityWorldSpaceLightDir(wPos.xyz));

		 
		 
		 
		 
		 
		 

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
	 
	#define V2F_SHADOW_CASTER_NOPOS float3 vec : TEXCOORD0;
	#define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
	#define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
	#define SHADOW_CASTER_FRAGMENT(i) return UnityEncodeCubeShadowDepth ((length(i.vec) + unity_LightShadowBias.x) * _LightPositionRange.w);
#else
	 
	#define V2F_SHADOW_CASTER_NOPOS
	 
	 
	#define V2F_SHADOW_CASTER_NOPOS_IS_EMPTY
	#define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) \
		opos = UnityObjectToClipPos(v.vertex.xyz); \
		opos = UnityApplyLinearShadowBias(opos);
	#define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) \
		opos = UnityClipSpaceShadowCasterPos(v.vertex, v.normal); \
		opos = UnityApplyLinearShadowBias(opos);
	#define SHADOW_CASTER_FRAGMENT(i) return 0;
#endif

 
 
#define V2F_SHADOW_CASTER V2F_SHADOW_CASTER_NOPOS float4 pos : SV_POSITION

 
 
#define TRANSFER_SHADOW_CASTER_NORMALOFFSET(o) TRANSFER_SHADOW_CASTER_NOPOS(o,o.pos)


#endif  


 
 

struct VS_OUT {
	vec3 normal;	 
};

#ifdef VERTEX
	layout (location = PositionIndex) 	in vec3 InputPositon;
	layout (location = NormalIndex)		in vec3 InputNormal;

	out VS_OUT vs_out;

	void main()
	{
		vec4 position = vec4(InputPositon, 1);
		vec3 normal = InputNormal;
		 
		 
		 
		gl_Position = position;
		vs_out.normal = normalize(normal);
	}
#endif

#ifdef GEOMETRY
	layout(triangles, invocations = 4) in;
	layout(triangle_strip, max_vertices = 3) out;

	in VS_OUT vs_out[];
	
	float4 ClipSpaceShadowCasterPos(float4 vertex, float3 normal, float biasScale)
	{
		float4 wPos = mul(MATRIX_M, vertex);

		if (unity_LightShadowBias.z != 0.0)
		{
			float3 wNormal = UnityObjectToWorldNormal(normal);
			float3 wLight = normalize(UnityWorldSpaceLightDir(wPos.xyz));

			 
			 
			 
			 
			 
			 

			float shadowCos = dot(wNormal, wLight);
			float shadowSine = sqrt(1-shadowCos*shadowCos);
			float normalBias = unity_LightShadowBias.z * biasScale * shadowSine;

			wPos.xyz -= wNormal * normalBias;
		}

		 
		return wPos;
	}

	float4 ApplyLinearShadowBias(float4 clipPos, float biasScale)
	{
		float bias = unity_LightShadowBias.x * biasScale;
	#if defined(UNITY_REVERSED_Z)
		 
		 
		clipPos.z += max(-1, min(bias / clipPos.w, 0));
		float clamped = min(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
	#else
		clipPos.z += saturate(bias/clipPos.w);
		float clamped = max(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
	#endif
		clipPos.z = lerp(clipPos.z, clamped, unity_LightShadowBias.y);
		return clipPos;
	}

	#define SHOWMAP_NO_BIAS

	void main()
	{
		for (int i = 0; i < gl_in.length(); ++i)
		{
		#ifdef SHOWMAP_NO_BIAS
			vec4 position = LightMatrix[gl_InvocationID] * MATRIX_M * gl_in[i].gl_Position;
			gl_Position = position;
		#else
			float4x4 matrixVP = LightMatrix[gl_InvocationID];
			 
			 
			float4 oPosition = gl_in[i].gl_Position;
			float3 oNormal = normalize(vs_out[i].normal);
			float4 wPos = ClipSpaceShadowCasterPos(oPosition, oNormal, 0.01);
			float4 cPos = matrixVP * wPos;
			gl_Position = ApplyLinearShadowBias(cPos, 0.1);
		#endif
			gl_Layer = gl_InvocationID;
			EmitVertex();
		}
		EndPrimitive();
	}
#endif

#ifdef FRAGMENT
	void main()
	{
	}
#endif
