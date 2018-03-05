class Rect:
    def __init__(self):
        super().__init__()
        self.m_XMin = 0.0
        self.m_YMin = 0.0
        self.m_Width = 0.0
        self.m_Height = 0.0

    @property
    def x(self)->float:
        return self.m_XMin
    @x.setter
    def x(self, value:float):
        self.m_XMin = value
