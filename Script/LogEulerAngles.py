from FishEngine import Script

class LogEulerAngles(Script):
    def __init__(self):
        super().__init__();

    def Update(self):
        print(self.transform.localToWorldMatrix)
        print(self.transform.localEulerAngles, self.transform.localRotation)
