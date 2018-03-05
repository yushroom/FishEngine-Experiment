import FishEngineInternal

class __Quaternion:
    def __str__(self):
        return "Quaternion({0}, {1}, {2}, {3})".format(self.x, self.y, self.z, self.w)

Quaternion = FishEngineInternal.Quaternion
Quaternion.__str__ = __Quaternion.__str__

# class Quaternion:
#     def __init__(self, x:float, y:float, z:float, w:float):
#         self.x = x
#         self.y = y
#         self.z = z
#         self.w = w

#     def __str__(self):
#         return "Quaternion({0}, {1}, {2}, {3})".format(self.x, self.y, self.z, self.w)

#     @staticmethod
#     def Wrap(q):
#         # q: FishEngineInternal.Quaternion
#         return Quaternion(q.x, q.y, q.z, q.w);
