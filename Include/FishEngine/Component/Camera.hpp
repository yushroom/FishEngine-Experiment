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
		DeclareObject(Camera, 20);

		Camera() : Behaviour(Camera::ClassID, ClassName)
		{
			LOGF;
		}
		
		~Camera()
		{
			LOGF;
		}

		// The aspect ratio (width divided by height).
		float GetAspect() const
		{
//			return m_aspect;
			return Screen::GetAspect();
		}

		float GetFarClipPlane() const { return m_FarClipPlane; }
		void SetFarClipPlane(float value) { m_FarClipPlane = value; }
		
		
		float GetNearClipPlane() const { return m_NearClipPlane; }
		void SetNearClipPlane(float value) { m_NearClipPlane = value; }
		
		
		float GetFieldOfView() const { return m_FieldOfView; }
		void SetFieldOfView(float value) { m_FieldOfView = value; }
		
		
		bool GetOrthographic() const { return m_Orthographic; }
		void SetOrthographic(bool value) { m_Orthographic = value; }
		
		
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float value) { m_OrthographicSize = value; }

		
		Frustum GetFrustum() const
		{
			return {m_FieldOfView, m_FarClipPlane, m_NearClipPlane, this->GetAspect()};
		}

		// Matrix that transforms from world to camera space (i.e. view matrix).
		Matrix4x4 GetWorldToCameraMatrix() const;

		Matrix4x4 GetCameraToWorldMatrix() const;

		// projection matrix.
		const Matrix4x4& GetProjectionMatrix() const;

		const Vector4& GetViewport() const
		{
			return m_viewport;
		}

//		void ResetAspect();

		// Returns a ray going from camera through a screen point.
		Ray ScreenPointToRay(const Vector3& position);

//		void virtual OnDrawGizmos() override;
//		void virtual OnDrawGizmosSelected() override;

		// The first enabled camera tagged "MainCamera" (Read Only).
		static Camera* GetMainCamera();

		static void OnWindowSizeChanged(const int width, const int height);


	private:
//		friend class RenderSystem;

		float 			m_FarClipPlane        = 1000.f;
		float 			m_NearClipPlane       = 0.3f;
		float 			m_FieldOfView         = 60.f;
		bool  			m_Orthographic = false;
		// Projection's half-size(vertical) when in orthographic mode.
		float 			m_OrthographicSize    = 5.f;
		
		
		// The aspect ratio (width divided by height).
		mutable float 	m_aspect              = 0;
//		bool m_isAspectSet         = false;
		Vector4 		m_viewport{ 0, 0, 1, 1 };
		mutable bool 	m_isDirty = true;

		CameraType  	m_cameraType = CameraType::Game;

//		Meta(NonSerializable)
		mutable Matrix4x4 m_projectMatrix;

//		static CameraPtr m_mainCamera;
//		static std::vector<CameraPtr> m_allCameras;
	};
}

#endif /* Camera_hpp */
