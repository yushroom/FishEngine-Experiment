from . import Vector3

class RaycastHit:
    pass

class Physics:
    @staticmethod
    def Ryacast(origin:Vector3, direction:Vector3, maxDistance:float, layermask:int) -> bool:
        pass