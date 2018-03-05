from FishEngine import Script

class Rotator(Script):
    def __init__(self):
        super().__init__();

    def Start(self):
        pass

    def Update(self):
        eulerAngles = self.transform.localEulerAngles
        # print("eulerAngles:", eulerAngles)
        eulerAngles.y += 2.0
        self.transform.localEulerAngles = eulerAngles
        # self.transform.eulerAngles.y += 2.0