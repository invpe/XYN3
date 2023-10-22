//
#include "xmain.h"
 

namespace X
{  

  
	Shader::Shader()
	{
		 
	}

bool Shader::Reload()
{

		return loadShader(s_vertexName,s_fragmentName);
} 
/*
*	Returns uniform location from our list
*	Shader optimization - do not generate glGet calls to uniform locations
*	rev 1013
*/
int Shader::getUniform(const std::string &uniformName)
{
	
	//Find the requestes unfirom name on our list
	int iter=0;
	for(iter=0;iter<uniformLocations.size();iter++)
	{

		if(uniformLocations[iter]->uniformName==uniformName)
			return uniformLocations[iter]->location;
	 
	}


	//If we're here, it means location has not been found, give it a try
	//And ask GL where it is
	int location=glGetUniformLocation(m_hProgramObject, uniformName.c_str());
	if(location>=0)
	{
		//Found, lets add it to our list
		uniformLoc *new_Ulocation=new uniformLoc;
		new_Ulocation->location=location;
		new_Ulocation->uniformName=uniformName;
		uniformLocations.push_back(new_Ulocation);
		//Quit finally
		return location;
		
	}
	

//	Game->Log()->write("<%s:%i %s> [%s/%s] Warrning, uniform location \"%s\" not found.",__FILE__,__LINE__,__FUNCTION__,s_fragmentName.c_str(),s_vertexName.c_str(),uniformName.c_str());
	return -1;

	 
}



string Shader::LoadTextFile(const std::string &strFile)
{
	// Open the file passed in
	ifstream fin(strFile.c_str());

	if(!fin)
	{
 
		return "";
	}
  
	string strLine = "";
	string strText = "";

	// Go through and store each line in the text file within a "string" object
	while(getline(fin, strLine))
	{
		strText = strText + "\n" + strLine;
	}

	// Close our file
	fin.close();

	// Return the text file's data
	return strText;
}



bool Shader::loadShader(const std::string &strVertex, const std::string &strFragment)
{
 	string strVShader, strFShader;
	Game->Log()->write("<%s:%i %s> %s %s initializing",__FILE__,__LINE__,__FUNCTION__,strVertex.c_str(),strFragment.c_str());
	
	 	 
  

	uniformLocations.clear();	 
	m_hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	m_hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	strVShader = LoadTextFile(strVertex);
	strFShader = LoadTextFile(strFragment);
	const char *szVShader = strVShader.c_str();
	const char *szFShader = strFShader.c_str();
	glShaderSourceARB(m_hVertexShader, 1, &szVShader, NULL);
	glShaderSourceARB(m_hFragmentShader, 1, &szFShader, NULL);


	glCompileShader(m_hVertexShader);	 
 
	 GLint isCompiled;
 	 glGetShaderiv (m_hVertexShader,GL_COMPILE_STATUS,&isCompiled);
	 if(isCompiled==GL_TRUE)
	 {
		Game->Log()->write("<%s:%i %s> %s Compiled. ",__FILE__,__LINE__,__FUNCTION__,strVertex.c_str());
	 }
	 else
	 {
		Game->Log()->write("<%s:%i %s> %s Failed to compile, dump below. ",__FILE__,__LINE__,__FUNCTION__,strVertex.c_str());
		int blen,slen;
		glGetShaderiv(m_hVertexShader, GL_INFO_LOG_LENGTH , &blen);
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(m_hVertexShader, blen, &slen, compiler_log);
		Game->Log()->write("%s",compiler_log);
		return false;
	 }
	 
	 glCompileShader(m_hFragmentShader);
	 glGetShaderiv (m_hFragmentShader,GL_COMPILE_STATUS,&isCompiled);
	 if(isCompiled==GL_TRUE)
	 {
		Game->Log()->write("<%s:%i %s> %s Compiled. ",__FILE__,__LINE__,__FUNCTION__,strFragment.c_str());
	 }
	 else
	 {
		Game->Log()->write("<%s:%i %s> %s Failed to compile, dump below. ",__FILE__,__LINE__,__FUNCTION__,strFragment.c_str());
		int blen,slen;
		glGetShaderiv(m_hVertexShader, GL_INFO_LOG_LENGTH , &blen);
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(m_hVertexShader, blen, &slen, compiler_log);
		Game->Log()->write("%s",compiler_log);
		return false;

	 }




	m_hProgramObject = glCreateProgramObjectARB();
	glAttachObjectARB(m_hProgramObject, m_hVertexShader);
	glAttachObjectARB(m_hProgramObject, m_hFragmentShader);

	/* Bind attribute indexes for later VAO usage*/
    /* Attribute locations must be setup before calling glLinkProgram. */
    glBindAttribLocation(m_hProgramObject, 0, "VertexPosition");
    glBindAttribLocation(m_hProgramObject, 1, "VertexNormal");
	glBindAttribLocation(m_hProgramObject, 2, "VertexTexCoord");
	glBindAttribLocation(m_hProgramObject, 3, "VertexTangent");
 


	glLinkProgramARB(m_hProgramObject);
	int isLinked;
	glGetProgramiv (m_hProgramObject, GL_LINK_STATUS,&isLinked);
	 if(isLinked==GL_TRUE)
	 {
		
	 }
	 else
	 {
		Game->Log()->write("<%s:%i %s> Not linked, dump below ",__FILE__,__LINE__,__FUNCTION__);
		int blen,slen;
		glGetShaderiv(m_hVertexShader, GL_INFO_LOG_LENGTH , &blen);
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetInfoLogARB(m_hVertexShader, blen, &slen, compiler_log);
		Game->Log()->write("%s",compiler_log);
		return false;
		
	 }


 

	s_fragmentName=strFragment;
	s_vertexName=strVertex;








	Game->Log()->write("<%s:%i %s> Linked  %s %s",__FILE__,__LINE__,__FUNCTION__,s_fragmentName.c_str(),s_vertexName.c_str());
	 return true;
 }


GLint Shader::GetVariable(string strVariable)
{
	if(!m_hProgramObject)
		return -1;
	return glGetUniformLocation(m_hProgramObject, strVariable.c_str());
}
 

void Shader::Release()
{
	Game->Log()->write("<%s:%i %s> %s/%s Releasing. ",__FILE__,__LINE__,__FUNCTION__,s_fragmentName.c_str(),s_vertexName.c_str());
 
	if(m_hVertexShader)
	{
		glDetachObjectARB(m_hProgramObject, m_hVertexShader);
		glDeleteObjectARB(m_hVertexShader);
		m_hVertexShader = NULL;
	}
	if(m_hFragmentShader)
	{
		glDetachObjectARB(m_hProgramObject, m_hFragmentShader);
		glDeleteObjectARB(m_hFragmentShader);
		m_hFragmentShader = NULL;
	}
	if(m_hProgramObject)
	{
		glDeleteObjectARB(m_hProgramObject);
		m_hProgramObject = NULL;
	}
} 



}//ns