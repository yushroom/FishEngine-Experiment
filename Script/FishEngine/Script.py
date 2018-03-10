from . import Component
import FishEngineInternal
import weakref
from collections import defaultdict

from typing import List

class Script(Component):
    __slots__ = ()
    __scripts = defaultdict(weakref.WeakSet)
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.CreateScript()
        self.m_CachedPtr.SetPyObject(self)
        Script.__scripts[type(self)].add(self)

    def Awake(self):
        pass

    def Start(self):
        pass

    def Update(self):
        pass

    def FixedUpdate(self):
        pass

    def LateUpdate(self):
        pass

    def OnGUI(self):
        pass

    def OnDisable(self):
        pass

    def OnEnable(self):
        pass

    # for c++
    def GetVisiableAttributes(self)->List[str]:
        a = dir(self)
        b = dir(self.__class__)
        diff = set(a) - set(b)

        # __speed -> _Rotator__speed
        hidden_prefix = '_' + self.__class__.__name__ + '__'
        return [x for x in a if x in diff and not x.startswith(hidden_prefix)]
        # ret = [x for x in a and x not in b]
        # return ret

    # for c++
    def Clone(self)->'Script':
        print("Clone:", self)
        from copy import copy
        return copy(self)
# Script = FishEngineInternal.Script