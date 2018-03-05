#pragma once

#include <string>
#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Object.hpp>

namespace FishEngine
{
	enum class ShaderType
	{
		VertexShader,
		FragmentShader,
		GeometryShader,
	};
	
	class Shader : public Object
	{
	public:
		enum {ClassID = 48};
		
		Shader()
		{
			LOGF;
			Object::s_objects[ClassID].insert(this);
		}
		Shader(const Shader&) = delete;
		const Shader& operator=(const Shader&) = delete;
		
		~Shader();
		
		static Shader* FromString(const std::string& vs, const std::string& fs);
		
		void Use() const;
		
	private:
		friend class Graphics;
		unsigned int m_GLProgram = 0;
	};
}
