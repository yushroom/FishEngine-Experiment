class Color:
    def __init__(self, r:float, g:float, b:float, a:float = 1.0):
        self.r = r
        self.g = g
        self.b = b
        self.a = a

    def __str__(self):
        return "RGBA({}, {}, {}, {})".format(self.r, self.g, self.b, self.a)