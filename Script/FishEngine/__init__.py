import FishEngineInternal
# FishEngineInternal.Object.__slots__ = ()
# FishEngineInternal.Component.__slots__ = ()
# FishEngineInternal.Camera.__slots__ = ()

# def __getattr(self, attr):
#     raise AttributeError('\'Object\' object has no attribute \'%s\'' % attr)

# FishEngineInternal.Camera.__getattribute__ = __getattr

from FishEngine.TypeCheck import accepts, returns
from FishEngine.Debug import Debug
from FishEngine.Vector2 import Vector2
from FishEngine.Vector3 import Vector3
from FishEngine.Quaternion import Quaternion
from FishEngine.Matrix4x4 import Matrix4x4

from FishEngine.Object import Object
from FishEngine.Prefab import Prefab
from FishEngine.Component import Component
from FishEngine.Transform import Transform
from FishEngine.RectTransform import RectTransform
from FishEngine.GameObject import GameObject, PrimitiveType
from FishEngine.Script import Script

from FishEngine.Screen import Screen
from FishEngine.Camera import Camera
from FishEngine.Light import Light

from FishEngine.Mesh import Mesh
from FishEngine.MeshFilter import MeshFilter
from FishEngine.Shader import Shader
from FishEngine.Material import Material
from FishEngine.MeshRenderer import MeshRenderer
from FishEngine.Graphics import Graphics

from FishEngine.Scene import Scene, SceneManager

# ESC
from FishEngine.System import System

import FishEngine.UI