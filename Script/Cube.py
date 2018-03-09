from FishEngine import Script, Gizmos, Color, Vector3, Matrix4x4
from typing import List

from Slot import Slot

class Cube(Script):
	def __init__(self):
		super().__init__()
		self.slots:List(Slot) = [None]*4

	def OnDrawGizmos(self):
		Gizmos.matrix = self.transform.localToWorldMatrix
		Gizmos.color = Color.green
		Gizmos.DrawSphere(Vector3.zero, 0.2)

		Gizmos.matrix = Matrix4x4.identity
		for s in self.slots:
			if s.matched:
				Gizmos.DrawLine(s.transform.position, self.transform.position)