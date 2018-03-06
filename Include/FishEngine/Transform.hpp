#pragma once

#include "Component.hpp"
#include "Math/Vector3.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Matrix4x4.hpp"

#include "Scene.hpp"

#include <vector>

namespace FishEngine
{
    class Transform : public Component
    {
    public:
		
//		static constexpr int ClassID = 4;
		enum {ClassID = 4};

        Transform() : m_localRotation(0, 0, 0, 1), m_localPosition(0, 0, 0), m_localScale(1, 1, 1)
        {
            LOGF;
			Object::s_objects[ClassID].insert(this);
        }
		
		virtual ~Transform();
		
		// The position of the transform in world space.
		Vector3 position() const
		{
			UpdateMatrix();
			//return m_localToWorldMatrix.MultiplyPoint(0, 0, 0);
			auto& l2w = m_localToWorldMatrix;
			return Vector3(l2w.m[0][3], l2w.m[1][3], l2w.m[2][3]);
		}
		
		// position is in world space
		void setPosition(const Vector3& position)
		{
//			printf("Transform::setPosition %f %f %f\n", position.x, position.y, position.z);
			_setPosition(position.x, position.y, position.z);
		}
		
		void _setPosition(const float x, const float y, const float z)
		{
			if (m_parent != nullptr)
			{
				m_localPosition = m_parent->worldToLocalMatrix()._MultiplyPoint(x, y, z);
			}
			else
			{
				m_localPosition.Set(x, y, z);
			}
			MakeDirty();
		}
		
		
		// Position of the transform relative to the parent transform.
		Vector3 localPosition() const
		{
			return m_localPosition;
		}
		
		void setLocalPosition(const Vector3& position)
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
		Vector3 eulerAngles() const
		{
			return rotation().eulerAngles();
		}
		
		void setEulerAngles(const Vector3& eulerAngles)
		{
			this->setRotation(Quaternion::Euler(eulerAngles));
		}
		
		
		// The rotation as Euler angles in degrees relative to the parent transform's rotation.
		Vector3 localEulerAngles() const
		{
			return m_localRotation.eulerAngles();
		}
		
		void setLocalEulerAngles(const Vector3& eulerAngles)
		{
			m_localRotation.setEulerAngles(eulerAngles);
			MakeDirty();
		}
		
		void _setLocalEulerAngles(const float x, const float y, const float z)
		{
			setLocalEulerAngles(Vector3(x, y, z));
		}

		
		// The rotation of the transform in world space stored as a Quaternion.
		Quaternion rotation() const
		{
			UpdateMatrix();
			return m_localToWorldMatrix.ToRotation();
		}
		
		void setRotation(const Quaternion& new_rotation)
		{
			if (m_parent == nullptr)
			{
				m_localRotation = new_rotation;
			}
			else
			{
				m_localRotation = Quaternion::Inverse(m_parent->rotation()) * new_rotation;
			}
			MakeDirty();
		}
		
		
		// The rotation of the transform relative to the parent transform's rotation.
		Quaternion localRotation() const
		{
			return m_localRotation;
		}
		
		void setLocalRotation(const Quaternion& rotation)
		{
			m_localRotation = rotation;
			MakeDirty();
		}
		
		// The scale of the transform relative to the parent.
		Vector3 localScale() const
		{
			return m_localScale;
		}
		
		void setLocalScale(const Vector3& scale)
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

		// direction (1, 0, 0) in world space.
		Vector3 right() const
		{
			return rotation() * Vector3::right;
		}
		
		// direction (0, 1, 0) in world space
		Vector3 up() const
		{
			return rotation() * Vector3::up;
		}
		
		// direction (0, 0, 1) in world space.
		Vector3 forward() const
		{
			return rotation() * Vector3::forward;
		}
		
		void setForward(Vector3 const & newForward)
		{
			//this->LookAt(position() + newForward, this->up());
			setRotation(Quaternion::LookRotation(newForward));
		}
		
		Transform* GetParent() const
		{
			return m_parent;
		}
		
		// parent: The parent Transform to use.
		// worldPositionStays: If true, the parent-relative position, scale and rotation are modified such that the object keeps the same world space position, rotation and scale as before.
		void SetParent(Transform* parent, bool worldPositionStays = true);


		Matrix4x4 worldToLocalMatrix() const
		{
			UpdateMatrix();
//			return m_worldToLocalMatrix;
			return m_localToWorldMatrix.inverse();
		}
		
		
		// Matrix that transforms a point from local space into world space (Read Only).
		const Matrix4x4& localToWorldMatrix() const
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
		void SetRootOrder(int index)
		{
			assert(index >= 0);
			if (m_parent != nullptr)
			{
//				if (index >= m_parent->m_children.size())
//					index = m_parent->m_children.size()-1;
				assert(index <= m_parent->m_children.size());
			}
			if (index == m_RootOrder)
				return;
			int old = m_RootOrder;
			auto& c = m_parent == nullptr ?
						SceneManager::GetActiveScene()->m_rootTransforms :
						m_parent->m_children;
			c[index]->m_RootOrder = old;
			std::swap(c[index], c[old]);
			m_RootOrder = index;
		}
		
		const std::vector<Transform*>& GetChildren() const
		{
			return m_children;
		}

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
