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
    //color = InputNormal*0.5+0.5;
    normal = mat3(MATRIX_M) * InputNormal;
    normal = normalize(normal);
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

class Material(Object):
    __slots__ = ('__shader')

    __defaultMaterial:'Material' = None
    __errorMaterial:'Material' = None

    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.Material()
        self.__shader:Shader = None

    @staticmethod
    def CreateMaterial(vs:str, fs:str)->'Material':
        m = Material()
        m.shader = Shader.FromString(vs, fs)
        return m

    # def Clean(self):
    #     self.__shader.Clean()

    @staticmethod
    def StaticClean():
        # del Material.defaultMaterial
        # del Material.errorMaterial
        Material.__defaultMaterial = None
        Material.__errorMaterial = None

    @property
    def shader(self)->Shader:
        return self.__shader
    @shader.setter
    def shader(self, value:Shader):
        self.__shader = value
        self.m_CachedPtr.shader = value.m_CachedPtr

    @staticmethod
    def defaultMaterial()->'Material':
        # print('defaultMaterial')
        if Material.__defaultMaterial is None:
            Material.__defaultMaterial = Material.CreateMaterial(_vs, _fs)
        return Material.__defaultMaterial
    
    @staticmethod
    def errorMaterial()->'Material':
        if Material.__errorMaterial is None:
            Material.__errorMaterial = Material.CreateMaterial(_vs2, _fs2)
        return Material.__errorMaterial


# Material.defaultMaterial:Material = Material.CreateMaterial(_vs, _fs)
# Material.errorMaterial:Material = Material.CreateMaterial(_vs2, _fs2)


# Material = FishEngineInternal.Material

# class __Material:
#     @staticmethod
#     def CreateMaterial(vs:str, fs:str)->'Material':
#         m = Material()
#         print(1)
#         m.shader = Shader.FromString(vs, fs)
#         print(2)
#         return m

# print(3)
# Material.defaultMaterial = __Material.CreateMaterial(_vs, _fs)
# print(4)
# Material.errorMaterial = __Material.CreateMaterial(_vs2, _fs2)
# print(5)