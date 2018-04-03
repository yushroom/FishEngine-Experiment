#pragma once

#include "../Object.hpp"
#include "../Color.hpp"

namespace FishEngine
{
	enum class FogMode
	{
		Linear = 1,
		Exponential,
		ExponentialSquared	// default
	};

	enum class AmbientMode
	{
		Skybox,		// Skybox-based or custom ambient lighting.
		Trilight,	// Trilight ambient lighting.
		Flat = 3,	// Flat ambient lighting.
		Custom,		// Ambient lighting is defined by a custom cubemap.
	};

	class Material;
	class Light;

	class RenderSettings : public Object
	{
	public:
		DeclareObject(RenderSettings, 104);
		RenderSettings() : Object(ClassID, ClassName) {

		}

		bool GetFog() const { return m_Fog; }
		void SetFog(bool value) { m_Fog = value; }

		const Color& GetFogColor() const { return m_FogColor; }
		void SetFogColor(const Color& value) { m_FogColor = value; }

		const FogMode& GetFogMode() const { return m_FogMode; }
		void SetFogMode(const FogMode& value) { m_FogMode = value; }

		float GetFogDensity() const { return m_FogDensity; }
		void SetFogDensity(float value) { m_FogDensity = value; }

		float GetLinearFogStart() const { return m_LinearFogStart; }
		void SetLinearFogStart(float value) { m_LinearFogStart = value; }

		float GetLinearFogEnd() const { return m_LinearFogEnd; }
		void SetLinearFogEnd(float value) { m_LinearFogEnd = value; }

		const Color& GetAmbientSkyColor() const { return m_AmbientSkyColor; }
		void SetAmbientSkyColor(const Color& value) { m_AmbientSkyColor = value; }

		const Color& GetAmbientEquatorColor() const { return m_AmbientEquatorColor; }
		void SetAmbientEquatorColor(const Color& value) { m_AmbientEquatorColor = value; }

		const Color& GetAmbientGroundColor() const { return m_AmbientGroundColor; }
		void SetAmbientGroundColor(const Color& value) { m_AmbientGroundColor = value; }

		float GetAmbientIntensity() const { return m_AmbientIntensity; }
		void SetAmbientIntensity(float value) { m_AmbientIntensity = value; }

		const AmbientMode& GetAmbientMode() const { return m_AmbientMode; }
		void SetAmbientMode(const AmbientMode& value) { m_AmbientMode = value; }

		const Color& GetSubtractiveShadowColor() const { return m_SubtractiveShadowColor; }
		void SetSubtractiveShadowColor(const Color& value) { m_SubtractiveShadowColor = value; }

		Material* GetSkyboxMaterial() const { return m_SkyboxMaterial; }
		void SetSkyboxMaterial(Material* value) { m_SkyboxMaterial = value; }

		float GetHaloStrength() const { return m_HaloStrength; }
		void SetHaloStrength(float value) { m_HaloStrength = value; }

		float GetFlareStrength() const { return m_FlareStrength; }
		void SetFlareStrength(float value) { m_FlareStrength = value; }

		float GetFlareFadeSpeed() const { return m_FlareFadeSpeed; }
		void SetFlareFadeSpeed(float value) { m_FlareFadeSpeed = value; }

		Light* GetSun() const { return m_Sun; }
		void SetSun(Light* value) { m_Sun = value; }

		const Color& GetIndirectSpecularColor() const { return m_IndirectSpecularColor; }
		void SetIndirectSpecularColor(const Color& value) { m_IndirectSpecularColor = value; }

	private:
		bool m_Fog = false;
		Color m_FogColor {0.5, 0.5, 0.5};
		FogMode m_FogMode = FogMode::ExponentialSquared;

		// The density of the exponential fog.
		float m_FogDensity = 0.01f;
		float m_LinearFogStart = 0;
		float m_LinearFogEnd = 300;

		Color m_AmbientSkyColor  {0.212f, 0.227f, 0.259f, 1};
		Color m_AmbientEquatorColor  {0.114f, 0.125f, 0.133f, 1};
		Color m_AmbientGroundColor  {0.047, 0.043, 0.035, 1};

		// How much the light from the Ambient Source affects the scene.
		float m_AmbientIntensity = 1;

		// Ambient lighting mode.
		AmbientMode m_AmbientMode = AmbientMode::Skybox;

		// The color used for the sun shadows in the Subtractive lightmode.
		Color m_SubtractiveShadowColor {0.42f, 0.478f, 0.627f, 1};

		Material* m_SkyboxMaterial = nullptr;

		// Size of the Light halos.
		float m_HaloStrength = 0.5f;
		float m_FlareStrength = 1;
		float m_FlareFadeSpeed = 3;
//		m_HaloTexture: {fileID: 0}
//		m_SpotCookie: {fileID: 10001, guid: 0000000000000000e000000000000000, type: 0}

//		m_DefaultReflectionMode: 0
//		m_DefaultReflectionResolution: 128
//		m_ReflectionBounces: 1
//		m_ReflectionIntensity: 1
//		m_CustomReflection: {fileID: 0}

		Light* m_Sun = nullptr;
		Color m_IndirectSpecularColor {0.44657826f, 0.49641263f, 0.57481676f, 1};
	};
}