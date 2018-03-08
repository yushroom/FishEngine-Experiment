#ifndef Camera_hpp
#define Camera_hpp

#include "Behaviour.hpp"
#include "../Math/Matrix4x4.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Frustum.hpp"
#include "../Screen.hpp"

namespace FishEngine 
{
	enum class CameraType
	{
		Game,
		SceneView,
		Preview,
	};

	class FE_EXPORT Camera : public Behaviour
	{
	public:
		enum {ClassID = 20};
//		DefineComponent(Camera)
		Camera() : Behaviour(Camera::ClassID)
		{
			LOGF;
		}
		//Camera(float fov, float nearClipPlane, float farClipPlane);
		
		~Camera()
		{
			LOGF;
		}

		// The aspect ratio (width divided by height).
		float aspect() const
		{
//			return m_aspect;
			return Screen::GetAspect();
		}

		float fieldOfView() const
		{
			return m_fieldOfView;
		}

		float orthographicSize() const
		{
			return m_orthographicSize;
		}
		
		void setOrthographicSize(float size)
		{
			m_orthographicSize = size;
			m_isDirty = true;
		}

		bool GetOrthographic() const
		{
			return m_orthographic;
		}

		void SetOrthographic(bool value)
		{
			m_orthographic = value;
			m_isDirty = true;
		}

//		void setAspect(float aspect)
//		{
//			m_aspect = aspect;
//			m_isDirty = true;
//			m_isAspectSet = true;
//		}

		void setFieldOfView(float fieldOfView)
		{
			m_fieldOfView = fieldOfView;
			m_isDirty = true;
		}

		float nearClipPlane() const
		{
			return m_nearClipPlane;
		}

		void setNearClipPlane(const float nearClipPlane)
		{
			m_nearClipPlane = nearClipPlane;
			m_isDirty = true;
		}

		float farClipPlane() const
		{
			return m_farClipPlane;
		}

		void setFarClipPlane(const float farClipPlane)
		{
			m_farClipPlane = farClipPlane;
			m_isDirty = true;
		}
		
		Frustum frustum() const
		{
			return {m_fieldOfView, m_farClipPlane, m_nearClipPlane, this->aspect()};
		}

		// Matrix that transforms from world to camera space (i.e. view matrix).
		Matrix4x4 worldToCameraMatrix() const;

		// projection matrix.
		const Matrix4x4& projectionMatrix() const;

		const Vector4& viewport() const
		{
			return m_viewport;
		}

//		void ResetAspect();

		// Returns a ray going from camera through a screen point.
		Ray ScreenPointToRay(const Vector3& position);

//		void virtual OnDrawGizmos() override;
//		void virtual OnDrawGizmosSelected() override;

		// TODO
		// The first enabled camera tagged "MainCamera" (Read Only).
//		static CameraPtr main();

//		static CameraPtr mainGameCamera();
		
//		// TODO: delete it
//		static void setMainCamera(CameraPtr camera)
//		{
//			m_mainCamera = camera;
//		}

		static void OnWindowSizeChanged(const int width, const int height);

//		static Camera*
//		Create(
//			float       fov             = 60,
//			float       nearClipPlane   = 0.3f,
//			float       farClipPlane    = 1000.0f,
//			CameraType  type            = CameraType::Game);

	private:
//		friend class RenderSystem;

		float m_fieldOfView         = 60.f;

		// Projection's half-size(vertical) when in orthographic mode.
		float m_orthographicSize    = 5.f;

		// The aspect ratio (width divided by height).
		mutable float m_aspect              = 0;
//		bool m_isAspectSet         = false;

		float m_farClipPlane        = 1000.f;
		float m_nearClipPlane       = 0.3f;
		Vector4 m_viewport{ 0, 0, 1, 1 };
		mutable bool m_isDirty = true;

		CameraType  m_cameraType = CameraType::Game;
		bool        m_orthographic = false;

//		Meta(NonSerializable)
		mutable Matrix4x4 m_projectMatrix;

//		static CameraPtr m_mainCamera;
//		static std::vector<CameraPtr> m_allCameras;
	};
}

#endif /* Camera_hpp */
