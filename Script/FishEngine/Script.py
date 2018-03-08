from . import Component
# from . import FishEngineInternal
import FishEngineInternal
import weakref
from collections import defaultdict

class Script(Component):
    __slots__ = ()
    __scripts = defaultdict(weakref.WeakSet)
    def __init__(self):
        super().__init__()
        self.m_CachedPtr = FishEngineInternal.Script()
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

# Script = FishEngineInternal.Script