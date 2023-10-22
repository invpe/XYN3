#include "xmain.h"

namespace X
{




	CShadersManager::CShadersManager()
	{
	}

	CShadersManager::~CShadersManager()
	{
		int iter;
		for(iter=0;iter<Shaders.size();iter++)
		{
			Game->Log()->write("<%s:%i %s> Destroying Shader \"%s\"",__FILE__,__LINE__,__FUNCTION__,Shaders[iter]->s_shaderName.c_str());
			Shaders[iter]->~Shader();
		}
	}

	bool CShadersManager::reloadShaders()
	{
		Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);
		int iter;
		for(iter=0;iter<Shaders.size();iter++)
		{
			Game->Log()->write("<%s:%i %s> Reloading %s",__FILE__,__LINE__,__FUNCTION__,Shaders[iter]->s_shaderName.c_str());
			if(Shaders[iter]->Reload()==false)
			{
					Game->Log()->write("<%s:%i %s> Error, Reloading %s - Failed",__FILE__,__LINE__,__FUNCTION__,Shaders[iter]->s_shaderName.c_str());
					return false;
			}
			Game->Log()->write("<%s:%i %s> Reloading %s - Completed",__FILE__,__LINE__,__FUNCTION__,Shaders[iter]->s_shaderName.c_str());
		}

		return true;
	}
	
	bool CShadersManager::loadShader(const std::string &s_ShaderName)
	{
		Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_ShaderName.c_str());
		return loadShader(s_ShaderName,"","");
	}

	void CShadersManager::destroyShader(const std::string &s_ShaderName)
	{
		if(getShader(s_ShaderName)!=NULL)
		{
			getShader(s_ShaderName)->~Shader();
		}
	}

	Shader *CShadersManager::getShader(const std::string &s_ShaderName)
	{
		int iter;
		for(iter=0;iter<Shaders.size();iter++)
		{
			if(Shaders[iter]->s_shaderName==s_ShaderName)
			{				
				return Shaders[iter];
			}
		}
		return NULL;
	}


	bool CShadersManager::loadShader(const std::string &s_ShaderName,const std::string &s_optFragmentShader, const std::string &s_optVertexShader)
	{
		Game->Log()->write("<%s:%i %s> \"%s\" \"%s\" \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_ShaderName.c_str(),s_optFragmentShader.c_str(),s_optVertexShader.c_str());

		//Safety check
		if(s_ShaderName.empty()==true)return false;



		//Check if shader not duplicate
		int iter;
		for(iter=0;iter<Shaders.size();iter++)
		{
			if(Shaders[iter]->s_shaderName==s_ShaderName)
			{
				Game->Log()->write("<%s:%i %s> \"%s\" already on the list.",__FILE__,__LINE__,__FUNCTION__,s_ShaderName.c_str());
				return false;
			}
		}


		//New shader comes in
		Shader *newShader=new Shader;		

		//Assign unique name for this shader
		newShader->s_shaderName=s_ShaderName;

		std::string s_Vert;
		std::string s_Frag;
		
		//Load Vertex shader
		if(s_optVertexShader.empty()==true)
			s_Vert="data/shaders/default.vert";
		else
			s_Vert="data/shaders/"+s_optVertexShader+".vert";


		
		//Load Fragment shader
		if(s_optFragmentShader.empty()==true)
			s_Frag="data/shaders/"+s_ShaderName+".frag";
		else 
			s_Frag="data/shaders/"+s_optFragmentShader+".frag";




		//Load shader
		if(newShader->loadShader(s_Vert,s_Frag)==false)
		{
				Game->Log()->write("<%s:%i %s> \"%s\" Error loading shader.",__FILE__,__LINE__,__FUNCTION__,s_ShaderName.c_str());
				return false;
		}


		//Push back this shader onto the stack
		Shaders.push_back(newShader);
		Game->Log()->write("<%s:%i %s> \"%s\" Loaded ok, shaders managed: %i",__FILE__,__LINE__,__FUNCTION__,s_ShaderName.c_str(),Shaders.size());

		return true;
	}


}