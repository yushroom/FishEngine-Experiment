from . import Component
import FishEngineInternal
import weakref
from collections import defaultdict

from typing import List

class Script(FishEngineInternal.Script):
    # __slots__ = ()
    __scripts = defaultdict(set)
    ClassID = FishEngineInternal.ScriptClassID()
    def __init__(self):
        super().__init__()
        # self.m_CachedPtr = FishEngineInternal.CreateScript()
        self.SetPyObject(self)
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
        # b = dir(self.__class__)
        b = dir(Script)
        diff = set(a) - set(b)

        # __speed -> _Rotator__speed
        hidden_prefix = '_' + self.__class__.__name__ + '__'
        return [x for x in a if x in diff and not x.startswith(hidden_prefix)]
        # ret = [x for x in a and x not in b]
        # return ret

    # for c++
    def Clone(self)->'Script':
        from copy import deepcopy
        # memo = {}
        # memo[self.m_CachedPtr] = self.m_CachedPtr
        cpp = self.m_CachedPtr
        self.m_CachedPtr = None
        cloned = deepcopy(self)
        self.m_CachedPtr = cpp
        return cloned

    def Serialize(self, dumper):
        super(Script, self).Serialize(dumper)
        dumper.d('ScriptName', self.__class__.__name__)
        for a in self.GetVisiableAttributes():
            dumper.d(a, getattr(self, a))

    def Deserialize(self, loader):
        # super(script)
        for attr in self.GetVisiableAttributes():
            setattr(self, attr, loader[attr])

    @staticmethod
    def StaticClean():
        # print(Script.__scripts)
        Script.__scripts.clear()
        # print(Script.__scripts)

# def Script__init__(self):
#     # super(FishEngineInternal.Script, self).__init__()
#     self.SetPyObject(self)

# Script = FishEngineInternal.Script
# Script.__init__ = Script__init__