#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
//#include <FishEngine/Scene.hpp>
#include <FishEngine/Screen.hpp>
//#include <FishEngine/Gizmos.hpp>

namespace FishEngine
{
//	CameraPtr Camera::m_mainCamera = nullptr;
//	std::vector<CameraPtr> Camera::m_allCameras;


//	void Camera::OnWindowSizeChanged(const int width, const int height)
//	{
//		const float aspect = float(width) / height;
//		for (auto& cam : m_allCameras)
//		{
//			if (!cam->m_isAspectSet)
//			{
//				cam->m_aspect = aspect;
//				cam->m_isDirty = true;
//			}
//		}
//	}

//	Camera* Camera::Create(
//		float fov, 
//		float nearClipPlane, 
//		float farClipPlane, 
//		CameraType type /*= CameraType::Game*/)
//	{
////		auto camera = MakeShared<Camera>(fov, nearClipPlane, farClipPlane);
//		auto camera = new Camera(fov, nearClipPlane,farClipPlane);
//		camera->m_cameraType = type;
////		m_allCameras.push_back(camera);
//		return camera;
//	}

//	Camera::Camera(float fov, float zNear, float zFar)
//		: m_fieldOfView(fov), m_farClipPlane(zFar), m_nearClipPlane(zNear)
//	{
//		//m_focusPoint = transform()->position() + transform()->forward() * 5.f;
////		ResetAspect();
//	}


	const Matrix4x4& Camera::GetProjectionMatrix() const
	{
		float aspect = Screen::GetAspect();
		if (m_aspect != aspect)
		{
			m_isDirty = true;
			m_aspect = aspect;
		}
		if (m_isDirty) {
			if (m_Orthographic) {
				float w = m_aspect * m_OrthographicSize;
				m_projectMatrix = Matrix4x4::Ortho(-w, w, -m_OrthographicSize, m_OrthographicSize, m_NearClipPlane, m_FarClipPlane);
			}
			else {
				m_projectMatrix = Matrix4x4::Perspective(m_FieldOfView, m_aspect, m_NearClipPlane, m_FarClipPlane);
			}
			m_isDirty = false;
		}
		return m_projectMatrix;
	}


//	void Camera::ResetAspect()
//	{
//		m_aspect = Screen::aspect();
//		m_isAspectSet = false;
//	}

//	Ray Camera::ScreenPointToRay(const Vector3& position)
//	{
//		//http://antongerdelan.net/opengl/raycasting.html
//
//		// NDC space
//		float x = (2.0f * position.x) / Screen::width() - 1.0f;
//		float y = (2.0f * position.y) / Screen::height() - 1.0f;
//		Vector4 ray_clip(x, y, 1.f, 1.0f);
//
//		Vector4 ray_eye = projectionMatrix().inverse() * ray_clip;
//		ray_eye.z = 1.0f; // forward
//		ray_eye.w = 0.0f;
//
//		Vector4 ray_world_h = transform()->localToWorldMatrix() * ray_eye;
//		Vector3 ray_world(ray_world_h.x, ray_world_h.y, ray_world_h.z);
//		return Ray(transform()->position(), ray_world.normalized()); 
//	}

	Matrix4x4 Camera::GetWorldToCameraMatrix() const
	{
		return GetTransform()->GetWorldToLocalMatrix();
	}

//	void Camera::OnDrawGizmos()
//	{
//		Gizmos::DrawIcon(transform()->position(), "Camera");
//	}
//
//	void Camera::OnDrawGizmosSelected()
//	{
//		Gizmos::setColor(Color::white);
//		Gizmos::setMatrix(transform()->localToWorldMatrix());
//		Gizmos::DrawFrustum(transform()->position(), m_fieldOfView, m_farClipPlane, m_nearClipPlane, m_aspect);
//		Gizmos::setMatrix(Matrix4x4::identity);
//	}

//	CameraPtr Camera::main()
//	{
//		if (m_mainCamera == nullptr)
//		{
//			for (auto& c : m_allCameras)
//			{
//				if (c->tag() == "MainCamera")
//				{
//					m_mainCamera = c;
//					return m_mainCamera;
//				}
//			}
//		}
//		return m_mainCamera;
//	}
//
//
//	CameraPtr Camera::
//	mainGameCamera()
//	{
//		for (auto& c : m_allCameras)
//		{
//			if (c->tag() == "MainCamera" && c->m_cameraType == CameraType::Game)
//			{
//				return c;
//			}
//		}
//		return nullptr;
//	}

	Camera* Camera::Clone() const
	{
		abort();
//		auto cloned = new Camera();
//		cloned->m_cameraType = m_cameraType;
//		cloned->m_farClipPlane = m_farClipPlane;
//		cloned->m_nearClipPlane = m_nearClipPlane;
//		cloned->m_fieldOfView = m_fieldOfView;
//		cloned->m_orthographic = m_orthographic;
//		cloned->m_orthographicSize = m_orthographicSize;
//		return cloned;
	}
}
