from FishEngine import Script, Color, Gizmos, Matrix4x4, Vector3

class Slot(Script):
    def __init__(self):
        super().__init__()
        
        self.cube: 'Cube' = None
        self.matched: 'Slot' = None

    def OnDrawGizmos(self):
        if self.matched == None:
            Gizmos.color = Color.red
        else:
            Gizmos.color = Color.green
        
        Gizmos.matrix = self.transform.localToWorldMatrix
        Gizmos.DrawCube(Vector3.zero, Vector3.one * 0.1)
        Gizmos.matrix = Matrix4x4.identidy
