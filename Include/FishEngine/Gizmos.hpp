#pragma once

#include "FishEngine.hpp"
#include "Color.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"
#include "Math/Frustum.hpp"
//#include "GLEnvironment.hpp"
//#include "ReflectClass.hpp"

namespace FishEngine
{
	class SimpleMesh;
	class RenderSystem;

	// Gizmos are used to give visual debugging or setup aids in the scene view.
	class FE_EXPORT Gizmos
	{
	public:
		Gizmos() = delete;
		
		static Color color()
		{
			return s_color;
		}
		
		static void
		SetColor(const Color& color)
		{
			s_color = color;
		}

		static void
		setMatrix(const Matrix4x4& matrix)
		{
			s_matrix = matrix;
		}
		
		static void
		DrawCube(
			const Vector3& center,
			const Vector3& size);
		
		static void
		DrawIcon(
			const Vector3&      center,
			const std::string&  name,
			bool                allowScaling = true);
		
		static void
		DrawLine(
			const Vector3& from,
			const Vector3& to);
		
		
		static void
		DrawSphere(
			const Vector3& center,
			const float radius);
		
		
		static void
		DrawRay(const Ray& r);
		
		
		static void
		DrawRay(
			const Vector3& from,
			const Vector3& direction);

		static void
		DrawHalfCircle(
			const Vector3&  center,
			const float     radius,
			const Vector3&  dir1,
			const Vector3&  dir2);

		static void
		DrawCircle(
			const Vector3&  center,
			const float     radius,
			const Vector3&  direction = Vector3::up);

		static void
		DrawLight(
			const Vector3&  center,
			const Vector3&  direction);


		/*! brief Draw a camera frustum using the currently set Gizmos.matrix for it's location and rotation.
		 * @parm[center] The apex of the truncated pyramid. NOT used here!
		 */
		static void
		DrawFrustum(
			const Vector3&      center,
			const float         fov,
			const float         maxRange,
			const float         minRange,
			const float         aspect);
		
		static void
		DrawFrustum(
			const Frustum&      frustum);

		static void
		DrawWireCube(
			const Vector3&      center,
			const Vector3&      size);

		static void
		DrawWireSphere(
			const Vector3&      center,
			const float         radius);

		static void
		DrawWireCapsule(
			const Vector3&      center,
			const float         radius,
			const float         height);

	private:
		friend RenderSystem;
		
		static void
		DrawHalfWireSphere(
			const Vector3& center,
			const float radius,
			const Matrix4x4& modelMatrix = Matrix4x4::identity);
		
		// Sets the color for the gizmos that will be drawn next.
		static Color        s_color;

		// Set the gizmo matrix used to draw all gizmos.
		static Matrix4x4    s_matrix;

		static void Init();
		
		static SimpleMesh* s_circleMesh;
		static SimpleMesh* s_boxMesh;
		static SimpleMesh* s_light;
		//static PSimpleMesh s_wiredSphereMesh;
	};
}
