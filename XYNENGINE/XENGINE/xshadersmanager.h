#pragma once

namespace X
{
	

	class CShadersManager
	{
	public:
		CShadersManager();
		~CShadersManager();

		bool loadShader(const std::string &s_ShaderName);
		bool loadShader(const std::string &s_ShaderName,const std::string &s_optFragmentShader, const std::string &s_optVertexShader);
		Shader *getShader(const std::string &s_ShaderName);
		bool reloadShaders();
		void destroyShader(const std::string &s_ShaderName);
	private:
		std::vector<Shader*> Shaders; 
	};
}