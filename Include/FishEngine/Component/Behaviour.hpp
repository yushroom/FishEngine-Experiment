#pragma once

#include "../Component.hpp"
#include "../Debug.hpp"

namespace FishEngine
{
	// Behaviours are Components that can be enabled or disabled.
	class FE_EXPORT Behaviour : public Component
	{
	public:
		DeclareObject(Behaviour, 8);

		Behaviour(int classID, const char* className) : Component(classID, className)
		{

		}

		virtual ~Behaviour() = default;

		// Enabled Behaviours are Updated, disabled Behaviours are not.
		bool enabled() const { return m_Enabled; }
		void setEnabled(bool value) { m_Enabled = value; }

		// Has the Behaviour had enabled called.
		bool isActiveAndEnabled() const;

	private:
//		friend class FishEditor::Inspector;
		bool m_Enabled = true;
	};
}
