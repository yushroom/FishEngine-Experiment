from FishEngine import GameObject, HideFlags, Camera, Light, Color, Vector3
from . import EditorUtility
from typing import List

class SceneView:

    kSceneViewDownLight = Color(0.047, 0.043, 0.035, 1)
    kSceneViewFrontLight = Color(0.769, 0.769, 0.769, 1.0)
    kSceneViewUpLight = Color(0.212, 0.227, 0.259, 1.0)
    kSceneViewMidLight = Color(0.114, 0.125, 0.133, 1.0)

    def __init__(self):
        self.m_Camera: Camera = None
        self.m_Light: List[Light] = [None] * 4

    @property
    def camera(self):
        return self.m_Camera

    def CreateSceneCameraAndLights(self):
        gameObject:GameObject = EditorUtility.CreateGameObjectWithHideFlags('SceneCamera', HideFlags.HideAndDontSave, Camera)
        # gameObject.AddComponentInternal("FlareLayer")
        # gameObject.AddComponentInternal("HaloLayer")
        self.m_Camera = gameObject.GetComponent(Camera)
        self.m_Camera.enabled = False
        self.m_Camera.cameraType = CameraType.SceneView
        for i in range(4):
            gameObject2:GameObject = EditorUtility.CreateGameObjectWithHideFlags('SceneLight', HideFlags.HideAndDontSave, Light)
            light = gameObject2.GetComponent(Light)
            light.type = LightType.Directional
            light.intensity = 1.0
            light.enabled = False
            self.m_Light[i] = light
        self.m_Light[0].color = SceneView.kSceneViewFrontLight
        self.m_Light[1].color = SceneView.kSceneViewUpLight - SceneView.kSceneViewMidLight
        self.m_Light[1].transform.LookAt(Vector3.down)
        self.m_Light[1].renderMode = LightRenderMode.ForceVertex
        self.m_Light[2].color = SceneView.kSceneViewDownLight - SceneView.kSceneViewMidLight
        self.m_Light[2].transform.LookAt(Vector3.up)
        self.m_Light[2].renderMode = LightRenderMode.ForceVertex

        HandleUtility.handleMaterial.SetColor("_SkyColor", SceneView.kSceneViewUpLight * 1.5)
        HandleUtility.handleMaterial.SetColor("_GroundColor", SceneView.kSceneViewDownLight * 1.5)
        HandleUtility.handleMaterial.SetColor("_Color", SceneView.kSceneViewFrontLight * 1.5)

    def OnGUI(self):
        pass