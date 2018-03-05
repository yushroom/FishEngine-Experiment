#include <FishEngine/Transform.hpp>

namespace FishEngine
{
	Transform::~Transform()
	{
		m_children.clear();
		// TODO
//		SetParent(nullptr); // remove from parent
		
		Object::s_objects[ClassID].erase(this);
	}
	
	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		// step1: update position
		auto vector = this->position();
		auto rotation = Quaternion::AngleAxis(angle, axis);
		Vector3 vector2 = vector - point;
		vector2 = rotation * vector2;
		vector = point + vector2;
		setPosition(vector);
		
		// step2: update rotation
		m_localRotation = rotation * m_localRotation;
		//RotateAroundInternal(axis, angle);
		MakeDirty();
	}

	
	void Transform::UpdateMatrix() const
	{
		if (!m_isDirty)
			return;
#if 1
		m_localToWorldMatrix.SetTRS(m_localPosition, m_localRotation, m_localScale);
		if (m_parent != nullptr) {
			m_localToWorldMatrix = m_parent->localToWorldMatrix() * m_localToWorldMatrix;
		}
//		m_worldToLocalMatrix = m_localToWorldMatrix.inverse();
#else
		// TODO this version is not right, take a look to see where the bug is.
		// maybe in the TRS
		Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale, m_localToWorldMatrix, m_worldToLocalMatrix);
		if (!m_parent.expired()) {
			m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
			m_worldToLocalMatrix = m_worldToLocalMatrix * m_parent.lock()->worldToLocalMatrix();
		}
#endif
		m_isDirty = false;
	}
	
	
	void Transform::SetParent(Transform* parent, bool worldPositionStays)
	{
		auto old_parent = m_parent;
		if (parent == old_parent)
		{
			return;
		}
		
		// new parent can not be child of this
		auto p = parent;
		while (p != nullptr)
		{
			if (p == this)
			{
				puts("new parent can not be child of this");
				return;
			}
			p = p->GetParent();
		}
		
		
		// remove from old parent
		if (old_parent != nullptr)
		{
			//p->m_children.remove(this);
			// TODO: remove first, not remove all
			old_parent->m_children.erase(old_parent->m_children.begin()+m_RootOrder);
		}
		
		// old_parent.localToWorld * old_localToWorld = new_parent.localToWorld * new_localToWorld
		// ==> new_localToWorld = new_parent.worldToLocal * old_parent.localToWorld * old_localToWorld
		
		m_parent = parent;
		if (parent != nullptr)
		{
			parent->m_children.push_back(this);
			m_RootOrder = parent->m_children.size()-1;
		}
		
		if ( worldPositionStays )
		{
			Matrix4x4 mat = Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale);
			if (old_parent != nullptr)
				mat = old_parent->localToWorldMatrix() * mat;
			if (parent != nullptr)
				mat = parent->worldToLocalMatrix() * mat;
			Matrix4x4::Decompose(mat, &m_localPosition, &m_localRotation, &m_localScale);
		}
		//UpdateMatrix();
		MakeDirty();
	}
	
	
	void Transform::MakeDirty() const
	{
		if (!m_isDirty)
		{
			for (auto& c : m_children)
			{
				c->MakeDirty();
			}
			m_isDirty = true;
		}
	}
}
