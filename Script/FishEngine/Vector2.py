import math

class Vector2:
    def __init__(self, x:float=0.0, y:float=0.0):
        self.x:float = x
        self.y:float = y

    def __str__(self):
        return "Vector2({}, {})".format(self.x, self.y)

    def Set(self, x:float, y:float):
        self.x = x
        self.y = y

    @staticmethod
    def Wrap(v:'FishEngineInternal.Vector2'):
        return Vector2(v.x, v.y)

    @property
    def normalized(self)->'Vector2':
        v = Vector2(self.x, self.y)
        v.Normalize()
        return v

    @property
    def magnitude(self)->float:
        return math.sqrt(self.x*self.x + self.y*self.y)

    @staticmethod
    def zero():
        return Vector2(0.0, 0.0)


    def __truediv__(self, d:float)->'Vector2':
        return Vector2(self.x / d, self.y / d)
    

    def Normalize(self):
        magnitude = self.magnitude
        if magnitude > 9.99999974737875E-06:
            # self = self / magnitude
            inv_len = 1 / magnitude
            self.x *= inv_len
            self.y *= inv_len
        else:
            # self = Vector2.zero()
            self.Set(0.0, 0.0)

if __name__ == '__main__':
    zero = Vector2.zero()
    print(zero)

    a = Vector2(10, 10)
    print(a.magnitude)
    print(a.normalized)