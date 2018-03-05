import FishEngineInternal
Vector3 = FishEngineInternal.Vector3

# def Vector3__str__(self):
#     return "({0}, {1}, {2})".format(self.x, self.y, self.z)

# def Vectorback():
#     return Vector3(0, 0, -1)

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
    def back():
        return Vector3(0, 0, -1)

    # @property
    @staticmethod
    def down():
        return Vector3(0, -1, 0)

    # @property
    @staticmethod
    def forward():
        return Vector3(0, 0, 1)

    # @property
    @staticmethod
    def left():
        return Vector3(-1, 0, 0)

    # @property
    @staticmethod
    def negativeInfinity():
        pass

    # @property
    @staticmethod
    def one():
        return Vector3(1, 1, 1)

    # @property
    @staticmethod
    def positiveInfinity():
        pass

    # @property
    @staticmethod
    def right():
        return Vector3(1, 0, 0)

    # @property
    @staticmethod
    def up():
        return Vector3(0, 1, 0)

    # @property
    @staticmethod
    def zero():
        return Vector3(0, 0, 0)

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

Vector3.forward = __Vector3.forward
Vector3.up = __Vector3.up
Vector3.down = __Vector3.down
Vector3.back = __Vector3.back
Vector3.left = __Vector3.left
Vector3.right = __Vector3.right
Vector3.one = __Vector3.one
Vector3.zero = __Vector3.zero


# class Vector3:
    
#     def __init__(self, x:float=0, y:float=0, z:float=0):
#         self.x = x
#         self.y = y
#         self.z = z

#     def __str__(self):
#         return "({0}, {1}, {2})".format(self.x, self.y, self.z)

#     # @property
#     @staticmethod
#     def back():
#         return Vector3(0, 0, -1)

#     # @property
#     @staticmethod
#     def down():
#         return Vector3(0, -1, 0)

#     # @property
#     @staticmethod
#     def forward():
#         return Vector3(0, 0, 1)

#     # @property
#     @staticmethod
#     def left():
#         return Vector3(-1, 0, 0)

#     # @property
#     @staticmethod
#     def negativeInfinity():
#         pass

#     # @property
#     @staticmethod
#     def one():
#         return Vector3(1, 1, 1)

#     # @property
#     @staticmethod
#     def positiveInfinity():
#         pass

#     # @property
#     @staticmethod
#     def right():
#         return Vector3(1, 0, 0)

#     # @property
#     @staticmethod
#     def up():
#         return Vector3(0, 1, 0)

#     # @property
#     @staticmethod
#     def zero():
#         return Vector3(0, 0, 0)

#     @property
#     def magnitude(self):
#         return math.sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

#     @property
#     def normalized(self):
#         m = self.magnitude
#         return Vector3(self.x/m, self.y/m, self.z/m)

#     @property
#     def sqrMagnitude(self):
#         return self.x ** 2 + self.y ** 2 + self.z ** 2
        
#     def __add__(self, v):
#         return Vector3(self.x + v.x, self.y + v.y, self.z + v.z)
        
#     def __sub__(self, v):
#         return Vector3(self.x - v.x, self.y - v.y, self.z - v.z)

#     def __mul__(self, n):
#         return Vector3(self.x * n, self.y * n, self.z * n)
        
#     def __truediv__(self, n):
#         n /= -1
#         return self * n
        
#     @staticmethod
#     def Dot(v1, v2):
#         return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z)

#     @staticmethod
#     def Wrap(vec3):
#         # vec3: FishEngineInternal.Vector3
#         return Vector3(vec3.x, vec3.y, vec3.z)