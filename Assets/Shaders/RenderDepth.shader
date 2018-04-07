#ifdef VERTEX
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

layout (location = PositionIndex) in vec3 InputPosition;

void main()
{
    gl_Position = MATRIX_MVP * vec4(InputPosition, 1);
}

#endif


#ifdef FRAGMENT

void main()
{

}

#endif
