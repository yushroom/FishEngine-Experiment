import FishEngineInternal
Vector3 = FishEngineInternal.Vector3

# def Vector3__str__(self):
#     return "({0}, {1}, {2})".format(self.x, self.y, self.z)

import math

class __Vector3:
    def __init__(self, x:float=0, y:float=0, z:float=0):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "Vector3({0}, {1}, {2})".format(self.x, self.y, self.z)

    # @property
    @staticmethod
    def negativeInfinity():
        pass

    # @property
    @staticmethod
    def positiveInfinity():
        pass

    @property
    def magnitude(self):
        return math.sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

    @property
    def normalized(self):
        m = self.magnitude
        return Vector3(self.x/m, self.y/m, self.z/m)

    @property
    def sqrMagnitude(self):
        return self.x ** 2 + self.y ** 2 + self.z ** 2

Vector3.__str__ = __Vector3.__str__
