#include <FishEngine/Component/Camera.hpp>

using namespace FishEngine;

typedef bool AnimBool;
typedef float AnimFloat;
typedef Vector3 AnimVector3;
typedef Quaternion AnimQuaternion;

class EditorUtility
{
	static GameObject CreateGameObjectWithHideFlags(string name, HideFlags flags, params Type[] components)
	{
		GameObject gameObject = EditorUtility.Internal_CreateGameObjectWithHideFlags(name, flags);
		gameObject.AddComponent(typeof(Transform));
		foreach(Type componentType in components)
		{
			gameObject.AddComponent(componentType);
		}
		return gameObject;
	}
}

class SceneView
{
public:

	float size() const {
		return m_Size;
	}

	void size(float value) {
		if (value > 40000.f)
		{
			value = 40000.f;
		}
		m_Size = value;
	}

	float cameraDistance() const
	{
		if (!m_Camera.GetOrthographic())
		{
			//float num = this.m_Ortho.Fade(90f, 0f);
			float num = 90.0f;
			return size() / std::tanf(num * 0.5f * 0.0174532924f);
		}
		return size() / 2.0f;
	}

	Vector3 cameraTargetPosition() const
	{
		//return this.m_Position.target + this.m_Rotation.target * new Vector3(0f, 0f, this.cameraDistance);
		return m_Position + m_Rotation * Vector3(0.f, 0.f, cameraDistance());
	}

	Quaternion cameraTargetRotation()
	{
		return m_Rotation;
	}

	Camera& camera() {
		return m_Camera;
	}

private:

	CreateSceneCameraAndLights()
	{
		GameObject gameObject = EditorUtility.CreateGameObjectWithHideFlags("SceneCamera", HideFlags.HideAndDontSave, typeof(Camera));
		gameObject.AddComponentInternal("FlareLayer");
		gameObject.AddComponentInternal("HaloLayer");
		this.m_Camera = gameObject.GetComponent<Camera>();
		this.m_Camera.enabled = false;
		this.m_Camera.cameraType = CameraType.SceneView;
		for (int i = 0; i < 3; i++)
		{
			GameObject gameObject2 = EditorUtility.CreateGameObjectWithHideFlags("SceneLight", HideFlags.HideAndDontSave, typeof(Light));
			this.m_Light[i] = gameObject2.GetComponent<Light>();
			this.m_Light[i].type = LightType.Directional;
			this.m_Light[i].intensity = 1f;
			this.m_Light[i].enabled = false;
		}
		this.m_Light[0].color = SceneView.kSceneViewFrontLight;
		this.m_Light[1].color = SceneView.kSceneViewUpLight - SceneView.kSceneViewMidLight;
		this.m_Light[1].transform.LookAt(Vector3.down);
		this.m_Light[1].renderMode = LightRenderMode.ForceVertex;
		this.m_Light[2].color = SceneView.kSceneViewDownLight - SceneView.kSceneViewMidLight;
		this.m_Light[2].transform.LookAt(Vector3.up);
		this.m_Light[2].renderMode = LightRenderMode.ForceVertex;
		HandleUtility.handleMaterial.SetColor("_SkyColor", SceneView.kSceneViewUpLight * 1.5f);
		HandleUtility.handleMaterial.SetColor("_GroundColor", SceneView.kSceneViewDownLight * 1.5f);
		HandleUtility.handleMaterial.SetColor("_Color", SceneView.kSceneViewFrontLight * 1.5f);
		this.SetupPBRValidation();
	}


	void OnEnable();
	void OnGUI();


private:
	Camera m_Camera;
	AnimVector3 m_Ortho;
	AnimVector3 m_Position;
	AnimQuaternion m_Rotation;
	AnimFloat m_Size = 10.0f;
};