#pragma once

#include "Component.hpp"
#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Matrix4x4.hpp"

#include <vector>

namespace FishEngine
{
	class Transform : public Component
	{
		friend class GameObject;
	public:
		enum {ClassID = 4};

	protected:
		Transform() : Component(Transform::ClassID)
		{
			LOGF;
		}
		
	public:
		
		virtual ~Transform();
		
		// The position of the transform in world space.
		Vector3 GetPosition() const
		{
			UpdateMatrix();
			//return m_localToWorldMatrix.MultiplyPoint(0, 0, 0);
			auto& l2w = m_localToWorldMatrix;
			return Vector3(l2w.m[0][3], l2w.m[1][3], l2w.m[2][3]);
		}
		
		// position is in world space
		void SetPosition(const Vector3& position)
		{
//			printf("Transform::setPosition %f %f %f\n", position.x, position.y, position.z);
			_setPosition(position.x, position.y, position.z);
		}
		
		void _setPosition(const float x, const float y, const float z)
		{
			if (m_parent != nullptr)
			{
				m_localPosition = m_parent->GetWorldToLocalMatrix()._MultiplyPoint(x, y, z);
			}
			else
			{
				m_localPosition.Set(x, y, z);
			}
			MakeDirty();
		}
		
		
		// Position of the transform relative to the parent transform.
		Vector3 GetLocalPosition() const
		{
			return m_localPosition;
		}
		
		void SetLocalPosition(const Vector3& position)
		{
			m_localPosition = position;
			MakeDirty();
		}
		
		void _setLocalPosition(const float x, const float y, const float z)
		{
			m_localPosition.Set(x, y, z);
			MakeDirty();
		}
		
		
		// The rotation as Euler angles in degrees.
		Vector3 GetEulerAngles() const
		{
			return GetRotation().eulerAngles();
		}
		
		void SetEulerAngles(const Vector3& eulerAngles)
		{
			this->SetRotation(Quaternion::Euler(eulerAngles));
		}
		
		
		// The rotation as Euler angles in degrees relative to the parent transform's rotation.
		Vector3 GetLocalEulerAngles() const
		{
			return m_localRotation.eulerAngles();
		}
		
		void SetLocalEulerAngles(const Vector3& eulerAngles)
		{
			m_localRotation.setEulerAngles(eulerAngles);
			MakeDirty();
		}
		
		void _setLocalEulerAngles(const float x, const float y, const float z)
		{
			SetLocalEulerAngles(Vector3(x, y, z));
		}

		
		// The rotation of the transform in world space stored as a Quaternion.
		Quaternion GetRotation() const
		{
			UpdateMatrix();
			return m_localToWorldMatrix.ToRotation();
		}
		
		void SetRotation(const Quaternion& new_rotation)
		{
			if (m_parent == nullptr)
			{
				m_localRotation = new_rotation;
			}
			else
			{
				m_localRotation = Quaternion::Inverse(m_parent->GetRotation()) * new_rotation;
			}
			MakeDirty();
		}
		
		
		// The rotation of the transform relative to the parent transform's rotation.
		Quaternion GetLocalRotation() const
		{
			return m_localRotation;
		}
		
		void SetLocalRotation(const Quaternion& rotation)
		{
			m_localRotation = rotation;
			MakeDirty();
		}
		
		// The scale of the transform relative to the parent.
		Vector3 GetLocalScale() const
		{
			return m_localScale;
		}
		
		void SetLocalScale(const Vector3& scale)
		{
			m_localScale = scale;
			MakeDirty();
		}
		
		void _setLocalScale(const float x, const float y, const float z)
		{
			m_localScale.Set(x, y, z);
			MakeDirty();
		}
		
		void _setLocalScale(const float scale)
		{
			m_localScale.x = m_localScale.y = m_localScale.z = scale;
			MakeDirty();
		}
		
		// The global scale of the object(Read Only).
		Vector3 GetLossyScale()
		{
			auto p = GetParent();
			if (p != nullptr)
				return m_localScale * p->GetLossyScale();
			return m_localScale;
		}

		// direction (1, 0, 0) in world space.
		Vector3 GetRight() const
		{
			return GetRotation() * Vector3::right;
		}
		
		// direction (0, 1, 0) in world space
		Vector3 GetUp() const
		{
			return GetRotation() * Vector3::up;
		}
		
		// direction (0, 0, 1) in world space.
		Vector3 GetForward() const
		{
			return GetRotation() * Vector3::forward;
		}
		
		void SetForward(Vector3 const & newForward)
		{
			//this->LookAt(position() + newForward, this->up());
			SetRotation(Quaternion::LookRotation(newForward));
		}
		
		Transform* GetParent() const
		{
			return m_parent;
		}
		
		// parent: The parent Transform to use.
		// worldPositionStays: If true, the parent-relative position, scale and rotation are modified such that the object keeps the same world space position, rotation and scale as before.
		void SetParent(Transform* parent, bool worldPositionStays = true);


		Matrix4x4 GetWorldToLocalMatrix() const
		{
			UpdateMatrix();
//			return m_worldToLocalMatrix;
			return m_localToWorldMatrix.inverse();
		}
		
		
		// Matrix that transforms a point from local space into world space (Read Only).
		const Matrix4x4& GetLocalToWorldMatrix() const
		{
			UpdateMatrix();
			return m_localToWorldMatrix;
		}
		
		// Applies a rotation of zAngle degrees around the z axis, xAngle degrees around the x axis, and yAngle degrees around the y axis (in that order)
//		void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self);
		
		// Rotates the transform about axis passing through point in world coordinates by angle degrees.
		void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		
		
		void UpdateMatrix() const;
		
		int GetRootOrder() const { return m_RootOrder; }
		void SetRootOrder(int index);
		
		const std::vector<Transform*>& GetChildren() const
		{
			return m_children;
		}

		Transform* GetChildAt(int index) const
		{
			return m_children[index];
		}
		
		void SetSiblingIndex(float index);
		int GetSiblingIndex() const;
		
		Transform* Clone() const;

	protected:
		friend class GameObject;
		friend class Scene;
		
		Quaternion m_localRotation {0, 0, 0, 1};
		Vector3 m_localPosition {0, 0, 0};
		Vector3 m_localScale {1, 1, 1};
		
		std::vector<Transform*> m_children;
		Transform* m_parent = nullptr;
		int m_RootOrder = 0;
		
		mutable bool m_isDirty = true;
		mutable Matrix4x4 m_localToWorldMatrix;
//		mutable Matrix4x4 m_worldToLocalMatrix;
		
		void MakeDirty() const;
	};
}
