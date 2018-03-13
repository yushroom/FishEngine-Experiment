from FishEngine import HideFlags, GameObject

import FishEditorInternal

class EditorUtility:
    
    @staticmethod
    def CreateGameObjectWithHideFlags(name:str, flags:HideFlags, components):
        go:GameObject = FishEditorInternal.EditorUtility.CreateGameObjectWithHideFlags(name, flags)
        for c in components:
            go.AddComponent(c)
        return