#pragma once

#include "../Component.hpp"
#include "../Render/Material.hpp"
#include "../Math/Bounds.hpp"
#include <vector>

namespace FishEngine
{
	enum class ShadowCastingMode
	{
		Off = 0,
		On = 1,
		TwoSided = 2,
		ShdowsOnly = 3,
	};

	class Renderer : public Component
	{
	public:
		DeclareObject(Renderer, 25);

		Renderer(int classID, const char* className) : Component(classID, className)
		{
		}

		virtual ~Renderer() {}


		Material* GetMaterial() const
		{
			if (m_Materials.empty())
				return nullptr;
			return m_Materials[0];
		}

		void SetMaterial(Material* material)
		{
			if (m_Materials.empty())
				m_Materials.push_back(material);
			else
				m_Materials[0] = material;
		}

		void AddMaterial(Material* material)
		{
			m_Materials.push_back(material);
		}

//		virtual Bounds localBounds() const = 0;
//		Bounds bounds() const;


		//virtual void OnInspectorGUI() override;
//		virtual void OnDrawGizmosSelected() override;


		// Makes the rendered 3D object visible if enabled.
		bool GetEnabled() const { return m_Enabled; }

		// Makes the rendered 3D object visible if enabled.
		void SetEnabled(bool value) { m_Enabled = value; }

		const ShadowCastingMode& GetCastShadows() const { return m_CastShadows; }
		void SetCastShadows(const ShadowCastingMode& value) { m_CastShadows = value; }

		bool GetReceiveShadows() const { return m_ReceiveShadows; }
		void SetReceiveShadows(bool value) { m_ReceiveShadows = value; }

	protected:
		bool					m_Enabled = true;	// Makes the rendered 3D object visible if enabled.
		ShadowCastingMode		m_CastShadows = ShadowCastingMode::On;
		bool					m_ReceiveShadows = true;
		std::vector<Material*> 	m_Materials;
//		ShadowCastingMode		m_ShadowCastingMode = ShadowCastingMode::On;
	};
}