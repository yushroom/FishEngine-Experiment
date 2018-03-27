import FishEngineInternal
# FishEngineInternal.Object.__slots__ = ()
# FishEngineInternal.Component.__slots__ = ()
# FishEngineInternal.Camera.__slots__ = ()

# def __getattr(self, attr):
#     raise AttributeError('\'Object\' object has no attribute \'%s\'' % attr)

# FishEngineInternal.Camera.__getattribute__ = __getattr

from FishEngine.TypeCheck import accepts, returns
from .Debug import Debug
from .Vector2 import Vector2
from .Vector3 import Vector3
from .Quaternion import Quaternion
from .Matrix4x4 import Matrix4x4
from .Color import Color

from .Object import Object, HideFlags
from .Prefab import Prefab
from .Component import Component
from .Transform import Transform
from .RectTransform import RectTransform
from .GameObject import GameObject, PrimitiveType
from .Script import Script

from .Screen import Screen
from .Camera import Camera
from .Light import Light

from .Mesh import Mesh, MeshManager
from .MeshFilter import MeshFilter
from .Shader import Shader
from .Material import Material
from .MeshRenderer import MeshRenderer
from .Graphics import Graphics

from .Rigidbody import Rigidbody
from .Collider import Collider
from .BoxCollider import BoxCollider
from .SphereCollider import SphereCollider

from .Scene import Scene, SceneManager

# ESC
from .System import System

import FishEngine.UI