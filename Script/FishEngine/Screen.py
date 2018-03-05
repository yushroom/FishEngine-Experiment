import FishEngineInternal

class Screen:
    width = FishEngineInternal.Screen.width
    height = FishEngineInternal.Screen.height

    @staticmethod
    def SetResolution(width:int, height:int, fullscreen:bool, preferredRefreshRate:int=0):
        """Switches the screen resolution.
        """
        FishEngineInternal.Screen.SetResolution(width, height, fullscreen, preferredRefreshRate)