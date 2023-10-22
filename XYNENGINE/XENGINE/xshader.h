#ifndef _CSHADER_H
#define _CSHADER_H
#pragma once
#include <string>
#include <vector>


using namespace std;

 
#define APIENTRYP APIENTRY *
#define GL_VERTEX_SHADER_ARB              0x8B31
#define GL_FRAGMENT_SHADER_ARB            0x8B30
typedef char GLcharARB; 

namespace X
{  

	//Struktura opisujaca uniform w shaderze i jego miejsce
	struct uniformLoc
	{
		std::string uniformName;
		int location;
	};


class Shader
{
public:

	Shader();
	~Shader()	{ Release();	}

	
	bool	Reload();
	bool loadShader(const std::string &strVertex, const std::string &strFragment);
	int getUniform(const std::string &uniformName);

 

	void SetInt(GLint variable, int newValue)										{ glUniform1i(variable, newValue);		}	
	void SetMatrix4x4(GLint variable,const CMatrix4x4 &rMatrix4x4)					{ glUniformMatrix4fv(variable, 1, GL_FALSE, rMatrix4x4); }
	void SetMatrix3x3(GLint variable,const CMatrix3x3 &rMatrix3x3)					{ glUniformMatrix3fv(variable, 1, GL_FALSE, rMatrix3x3); }
 	void SetFloat(GLint variable, GLfloat newValue)									{ glUniform1f(variable, newValue);		}
	void SetVector2(GLint variable,const CVector2 &rCVector2)						{ glUniform2fARB(variable, rCVector2.x, rCVector2.y);	}
	void SetVector3(GLint variable,const CVector3 &rCVector3)						{ glUniform3fARB(variable, rCVector3.x, rCVector3.y,rCVector3.z);	}
	void SetVector4(GLint variable,const CVector4 &rCVector4)						{ glUniform4fARB(variable, rCVector4.x, rCVector4.y,rCVector4.z,rCVector4.w);	}

	void SetFloat2(GLint variable, GLfloat v0, GLfloat v1)							{ glUniform2fARB(variable, v0, v1);			}
	void SetFloat3(GLint variable, GLfloat v0, GLfloat v1, GLfloat v2)				{ glUniform3fARB(variable, v0, v1, v2);		}
	void SetFloat4(GLint variable, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)	{ glUniform4f(variable, v0, v1, v2, v3);	}
	void SetFloat3Array(GLint variable,GLfloat *var)								{ glUniform3fARB(variable, var[0],var[1],var[2]);	}
	
	
	void TurnOn()		{	glUseProgramObjectARB(m_hProgramObject); }
	void TurnOff()		{	glUseProgramObjectARB(0);				 }
	void Release();
	
	std::string s_shaderName;

private:
	GLhandleARB m_hVertexShader;
	GLhandleARB m_hFragmentShader;
	GLhandleARB m_hProgramObject;
	
	std::string s_fragmentName;
	std::string s_vertexName;
	

	 
 	string LoadTextFile(const std::string &strFile);
	GLint GetVariable(string strVariable);

	std::vector<uniformLoc*> uniformLocations;	//Lista uniformow w shaderze do ktorych sie odwolujemy i ich pozycja
};

 
  
}
#endif
 