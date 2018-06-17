# from . import Component
import FishEngineInternal
from typing import List

class Script:
    def __init__(self):
        print("Script.__init__", type(self))
        # super().__init__()    # do not call this
        self.m_CachedPtr = FishEngineInternal.CreateScript()
        self.m_CachedPtr.SetPyObject(self)

    def GetCachedPtr(self):
        return self.m_CachedPtr

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

    def Clone(self)->'Script':
        T = type(self)
        print('Script.Clone', T)
        return T()

    @property
    def gameObject(self)->FishEngineInternal.GameObject:
        return self.m_CachedPtr.GetGameObject()
    @gameObject.setter
    def gameObject(self, go:FishEngineInternal.GameObject):
        from . import GameObject
        assert(self.m_CachedPtr.gameObject is None)
        assert(isinstance(go, GameObject))
        go.AddComopnent(self.m_CachedPtr)

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

    def OnDisable(self):
        pass

    def OnEnable(self):
        pass

    def OnDrawGizmos(self):
        pass

    def OnDrawGizmosSelected(self):
        pass
