from . import Shader, Object
import FishEngineInternal


_vs = '''#version 410 core
#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5
layout (location = PositionIndex)   in vec3 InputPositon;
layout (location = NormalIndex)     in vec3 InputNormal;
layout (location = TangentIndex)    in vec3 InputTangent;
layout (location = UVIndex)         in vec2 InputUV;
uniform mat4 MATRIX_MVP;
uniform mat4 MATRIX_M;

out vec3 normal;

void main()
{
    gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
    //normal = InputNormal*0.5+0.5;
    normal = mat3(MATRIX_M) * InputNormal;
    //normal = normalize(normal);
    //normal = normalize(InputNormal);
    //color = vec3(gl_Position.z);
}
'''

_fs = '''#version 410 core
in vec3 normal;
//const vec3 L = normalize(vec3(1, 1, 0));
uniform vec3 LightDir;
out vec4 fragColor;
void main()
{
    vec3 N = normalize(normal);
    //fragColor = vec4(normal, 1);
    float ndotl = clamp(dot(N, -LightDir), 0, 1);
    fragColor = vec4(vec3(ndotl), 1);
    //fragColor = vec4(N*0.5+0.5, 1);
    //fragColor = vec4(vec3(gl_FragCoord.z*5), 1);
}
'''

_vs2 = '''#version 410 core
#define PositionIndex 0
layout (location = PositionIndex)   in vec3 InputPositon;
uniform mat4 MATRIX_MVP;

void main()
{
    gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
}
'''

_fs2 = '''#version 410 core
out vec4 fragColor;
void main()
{
    fragColor = vec4(1, 0, 1, 1);
}
'''

class __Material:
    @staticmethod
    def CreateMaterial(vs:str, fs:str)->'Material':
        m = Material()
        m.shader = Shader.FromString(vs, fs)
        return m

def Material__new__(cls):
    return FishEngineInternal.CreateMaterial()

def Material__init__(self):
    pass

Material = FishEngineInternal.Material
Material.__new__ = Material__new__
Material.__init__ = Material__init__
# Material.ClassID = FishEngineInternal.MaterialClassID()