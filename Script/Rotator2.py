from FishEngine import Script, Vector3

class Rotator2(Script):
    # __slots__ = ('speed', '__hidden')
    def __init__(self):
        super().__init__()
        self.speed = 1.0
        self.__hidden = True    # test

    # def SystemUpdate(self):
    def Update(self):
        # print(self.cpp.GetGameObject().name)
        self.transform.RotateAround(self.transform.parent.position, Vector3.up(), self.speed)
        # print(self.name, self.transform.localPosition, self.transform.position)
        # printT(self.transform)

# class Rotator2System(System):
#     def Update(self):
#         for go in GameObject.FindWithComponent(Rotator2):
#             go.GetComponent(Rotator2).SystemUpdate()