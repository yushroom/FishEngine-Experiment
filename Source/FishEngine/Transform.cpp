#include <FishEngine/Transform.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Scene.hpp>

namespace FishEngine
{
	Transform::~Transform()
	{
		LOGF;
//		for (auto c : m_children)
		for (int i = (int)m_children.size()-1; i >= 0; --i)
		{
			delete m_children[i]->GetGameObject();
		}
//		m_children.clear();
		// TODO
		SetParent(nullptr); // remove from parent
	}
	
	void Transform::SetRootOrder(int index)
	{
		assert(index >= 0);
		if (m_parent != nullptr)
		{
			// rootOrder in .unity file may > total size
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
	
	
	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		// step1: update position
		auto vector = this->GetPosition();
		auto rotation = Quaternion::AngleAxis(angle, axis);
		Vector3 vector2 = vector - point;
		vector2 = rotation * vector2;
		vector = point + vector2;
		SetPosition(vector);
		
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
			m_localToWorldMatrix = m_parent->GetLocalToWorldMatrix() * m_localToWorldMatrix;
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

		if (parent == nullptr)
			SceneManager::GetActiveScene()->AddRootTransform(this);
		else if (old_parent == nullptr)
			SceneManager::GetActiveScene()->RemoveRootTransform(this);
		
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
//			old_parent->m_children.erase(old_parent->m_children.begin()+m_RootOrder);
			std::remove(old_parent->m_children.begin(), old_parent->m_children.end(), this);
		}
		
		// old_parent.localToWorld * old_localToWorld = new_parent.localToWorld * new_localToWorld
		// ==> new_localToWorld = new_parent.worldToLocal * old_parent.localToWorld * old_localToWorld
		
		m_parent = parent;
		if (parent != nullptr)
		{
			parent->m_children.push_back(this);
			m_RootOrder = (int)parent->m_children.size()-1;
		}
		
		if ( worldPositionStays )
		{
			Matrix4x4 mat = Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale);
			if (old_parent != nullptr)
				mat = old_parent->GetLocalToWorldMatrix() * mat;
			if (parent != nullptr)
				mat = parent->GetWorldToLocalMatrix() * mat;
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
	
	Transform* Transform::Clone() const
	{
		auto cloned = new Transform();
		cloned->m_localPosition = this->m_localPosition;
		cloned->m_localRotation = this->m_localRotation;
		cloned->m_localScale = this->m_localScale;
		cloned->m_isDirty = true;
		return cloned;
	}
	
	void Transform::SetSiblingIndex(float index)
	{
		auto parent = GetParent();
		auto & children = parent != nullptr ?
			parent->m_children :
			m_gameObject->GetScene()->m_rootTransforms;
		
		assert(index >= 0 && index < children.size());
		
		auto pos = std::find(children.begin(), children.end(), this);
		int old_index = std::distance(children.begin(), pos);
//		int old_index = GetSiblingIndex();
		if (old_index < index)
		{
			// [old_index ... index] -> [old_index+1 ... index, old_index]
//			auto b = children.begin();
//			std::rotate(b+old_index, b+(old_index+1), b+index);
			
			for (int i = index-1; i >= old_index; --i)
			{
				children[i] = children[i+1];
			}
			children[index] = this;
		}
		else if (old_index > index)
		{
			// [index ... old_index] -> [old_index, index ... old_index-1]
			for (int i = old_index; i > index; --i)
			{
				children[i] = children[i-1];
			}
			children[index] = this;
		}
		m_RootOrder = index;
	}
	
	
	int Transform::GetSiblingIndex() const
	{
		auto parent = GetParent();
		auto & children = parent != nullptr ?
			parent->GetChildren() :
			m_gameObject->GetScene()->GetRootTransforms();
//		std::sort(children.begin(), children.end(), [](Transform* a, Transform* b) {
//			return a->m_RootOrder < b->m_RootOrder;
//		});
//
//		for (int i = 0; i < children.size(); ++i)
//		{
//			auto t = children[i];
//			t->m_RootOrder = i;
//		}
//		return m_RootOrder;
		auto pos = std::find(children.begin(), children.end(), this);
		return std::distance(children.begin(), pos);
	}
}
