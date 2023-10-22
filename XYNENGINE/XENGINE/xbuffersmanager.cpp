//
#include "xmain.h"
 
namespace X
{






	/*
	 *
	 *
	 *
	*/
	CBuffersManager::CBuffersManager()
	{
		Buffers.clear();
	}

	/*
	 *
	 *
	 *
	*/
	CBuffersManager::~CBuffersManager()
	{
	}

	/*
	 *
	 *
	 *
	*/
	int CBuffersManager::Lookup(const std::string &s_FBOObjectName,const std::string &s_RBOObjectName)
	{
		int advance=0;
	  	if(Buffers.empty()==true)return -1;


		/* Look for name in FBO and RBO */
		if(s_FBOObjectName.empty()==false && s_RBOObjectName.empty()==false)
		{


				for(advance=0;advance<Buffers.size();advance++)
				{
					if(Buffers[advance].s_FBO.c_str()==s_FBOObjectName && Buffers[advance].s_RBO.c_str()==s_RBOObjectName)
					{

						//Game->Log()->write("<%s:%i %s> Found FBO/RBO \"%s\".",__FILE__,__LINE__,__FUNCTION__,s_FBOObjectName.c_str());
						return advance;																												 

					}
				}


		}
		/* Look for name in FBO */
		else if(s_FBOObjectName.empty()==false && s_RBOObjectName.empty()==true)
		{
				for(advance=0;advance<Buffers.size();advance++)
				{
					if(Buffers[advance].s_FBO.c_str()==s_FBOObjectName )
					{

						//Game->Log()->write("<%s:%i %s> Found FBO \"%s\".",__FILE__,__LINE__,__FUNCTION__,s_FBOObjectName.c_str());
						return advance;																												 

					}
				}
		}
		/* Look for name in RBO */
		else if(s_FBOObjectName.empty()==true && s_RBOObjectName.empty()==false)
		{
				for(advance=0;advance<Buffers.size();advance++)
				{
					if(Buffers[advance].s_RBO.c_str()==s_RBOObjectName )
					{

						//Game->Log()->write("<%s:%i %s> Found RBO \"%s\".",__FILE__,__LINE__,__FUNCTION__,s_FBOObjectName.c_str());
						return advance;																												 

					}
				}
		}


		return -1;
	}
	 
	/*
	 *
	 *
	 *
	*/
	BufferData *CBuffersManager::get(int id)
	{


		return NULL;
		 
	}

	/*
	 *
	 *
	 *
	*/
	int CBuffersManager::BuffersCount()
	{

		
		return Buffers.size();
	}

	/*
	 *
	 *
	 * attach a renderbuffer as a logical buffer to the currently bound framebuffer object
	*/
	void CBuffersManager::AttachRenderbuffer(const std::string &s_RBOName,GLuint attachment)
	{

		//Attach renderbuffer		
		if(s_RBOName.empty()==false)
		{
		 int buffer=Lookup("",s_RBOName);
		 if(buffer>=0)
		 {
			 glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,attachment, GL_RENDERBUFFER_EXT, Buffers[buffer].h_RBO);	
		 }
		}

	}


	/*
	 *
	 *
	 * attach a texture to currently bound FBO
	*/
	void CBuffersManager::AttachTexture(const std::string &s_FBOName,GLuint attachment, GLuint texturebound,GLuint texturetype)
	{

		//TO FBO
		if(s_FBOName.empty()==false)
		{

			int buffer=Lookup(s_FBOName,"");
			if(buffer>=0)
			{			 
				glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachment, texturetype, texturebound, 0);		
				 
			}
 

		}

	

	}

	/*
	 *
	 *	unbind currently bound FBO
	 *
	*/
	void CBuffersManager::Unbind(const std::string &s_FBOObjectName,const std::string &s_RBOObjectName)
	{
					
		if(s_FBOObjectName.empty()==false)glBindFramebuffer( GL_FRAMEBUFFER, 0);		
		if(s_RBOObjectName.empty()==false)glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);		
		
	}

	/*
	 *
	 *
	 *
	*/
	bool CBuffersManager::Bind(const std::string &s_FBOName,const std::string &s_RBOName,int eventualBufferID)
	{


		//OPTION 1
		//If we have passsed the eventualBufferID, we don't look for the name, just bind what is required
		//But we still have to know what do we want to bind, so we use strings to see what was requested.
		if(eventualBufferID>=0)
		{
			if(s_FBOName.empty()==false)
			{
				glBindFramebuffer(GL_FRAMEBUFFER,Buffers[eventualBufferID].h_FBO);	
			}else return false;

			if(s_RBOName.empty()==false)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, Buffers[eventualBufferID].h_RBO);		
				
			}else return false;


			//RETURN OK
			return true;
		}







		//OPTION2 - we look for a name of buffer to bind
		//Bind FBO
		if(s_FBOName.empty()==false)
		{

			int buffer=Lookup(s_FBOName,"");
			if(buffer>=0)
			{

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,Buffers[buffer].h_FBO);	
			 
			}else
			return -1;


		}


		//Bind RBO
		if(s_RBOName.empty()==false)
		{

			int buffer=Lookup("",s_RBOName);
			if(buffer>=0)
			{

				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Buffers[buffer].h_RBO);		
		 
			}else
			return -1;

		}



		return 1;

	}
	CVector2 CBuffersManager::GetRBOSize(const std::string &s_RBOName)
	{
		int buffer=Lookup("",s_RBOName);
		if(buffer>=0)
		{
			return Buffers[buffer].sizeRBO;
		}
		return CVector2(0,0);
	}

	/*
	 *	Create(std::string s_FBOName,std::string s_RBOName,CVector2 vRBOsize,GLuint renderbufferComponent)
	 *	arguments	-	string s_FBOName - this informs of the name of the FBO to create, if exists - we will create FBO with that name
	 *				-	string s_RBOName - this informs of the name of the RBO to create, if exists - we will create RBO with that name
	 *				-   CVector2 vRBOsize - required if s_RBOName passed, informs of the RBO size
	 *				-	renderbufferComponent - required if s_RBOName passe DEPTH/COLOR component of the RBO 
	 *		
	 * synopsis		-	Creates an FBO/RBO or FBO or RBO or BOTH with provided data.
					-	Checks if FBO string is not empty, and creates FBO
					-	Checks if RBO string is not empty, and creates RBO
	*/
	int CBuffersManager::Create(const std::string &s_FBOName,const std::string &s_RBOName,CVector2 vRBOsize,GLuint renderbufferComponent)
	{

		BufferData newBuffer;
		


		//FRAME BUFFER
		if(s_FBOName.empty()==false)
		{


				Game->Log()->write("<%s:%i %s>  \"%s\" - FBO creation. ",__FILE__,__LINE__,__FUNCTION__,s_FBOName.c_str());

				glGenFramebuffersEXT( 1, &newBuffer.h_FBO );
				glBindFramebuffer( GL_FRAMEBUFFER_EXT, newBuffer.h_FBO );
				 

				if(newBuffer.h_FBO>0)
				{
					//Assign the name of the buffer
					newBuffer.s_FBO=s_FBOName;

					Game->Log()->write("<%s:%i %s>  \"%s\" - created ok. ",__FILE__,__LINE__,__FUNCTION__,s_FBOName.c_str());

				}
				else return -1;

		}


		//RENDER BUFFER
		if(s_RBOName.empty()==false)
		{
			Game->Log()->write("<%s:%i %s>  \"%s\" - RBO creation. ",__FILE__,__LINE__,__FUNCTION__,s_RBOName.c_str());

			glGenRenderbuffersEXT( 1, &newBuffer.h_RBO );	
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, newBuffer.h_RBO );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, renderbufferComponent, (int)vRBOsize.x,(int)vRBOsize.y);
		 
			if(newBuffer.h_RBO>0)
			{
					//Assign the name of the buffer
					newBuffer.s_RBO=s_RBOName;
					newBuffer.sizeRBO=vRBOsize;
					Game->Log()->write("<%s:%i %s>  \"%s\" - created ok. ",__FILE__,__LINE__,__FUNCTION__,s_RBOName.c_str());

			}
			else return -1;


		}





		//If any of the buffers requested are != false, we push them onto the stack
		if(s_RBOName.empty()==false || s_FBOName.empty()==false)
		{
		
				//if(CheckFrameBufferStatus()==true)
				{				
					Buffers.push_back(newBuffer);
					//And return the buffer internal list ID;
					Game->Log()->write("<%s:%i %s> Done. ",__FILE__,__LINE__,__FUNCTION__);
					return Buffers.size()-1;

				}
				
		}





		return -1;
	
	}	


	 

	bool CBuffersManager::CheckFrameBufferStatus()
	{
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				Game->Log()->write("<%s:%i %s> FBO OK.",__FILE__,__LINE__,__FUNCTION__);
			return true;
			break;
			
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_UNSUPPORTED_EXT status.",__FILE__,__LINE__,__FUNCTION__);
			break;

  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;

 
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;


    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;


    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;


    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;


    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT status.",__FILE__,__LINE__,__FUNCTION__);
        return false;
        break;

 
			default:
				Game->Log()->write("<%s:%i %s> ERROR - FBO  not complete: unknown error status.",__FILE__,__LINE__,__FUNCTION__);				
			break;
		}

		return false;
	}

}