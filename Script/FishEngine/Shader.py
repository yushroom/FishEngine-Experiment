from . import Object

import FishEngineInternal

class Shader(Object):
    __slots__ = ()
    def __init__(self):
        super().__init__()
        # self.m_CachedPtr = None

    @staticmethod
    def FromString(vs:str, fs:str)->'Shader':
        s = Shader()
        s.m_CachedPtr = FishEngineInternal.Shader.FromString(vs, fs)
        return s
        
# class __Shader:
#     @staticmethod
#     def default

# Shader = FishEngineInternal.Shader

