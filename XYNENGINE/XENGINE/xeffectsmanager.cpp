#include "xmain.h"
namespace X
{


	CEffectsManager::CEffectsManager()
	{
	 
	}


	CEffectsManager::~CEffectsManager()
	{
		Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);
		int effCt=0;
		for(effCt=0;effCt<xEffects.size();effCt++)
		{
			Game->Log()->write("<%s:%i %s> Destroying xEffect \"%s\"",__FILE__,__LINE__,__FUNCTION__,xEffects[effCt]->s_EffectName.c_str());
			Game->ShadersManager()->destroyShader(xEffects[effCt]->s_EffectName);
		}
	}
	bool CEffectsManager::isQueued(const std::string &s_EffectName)
	{
		int iter;
		for(iter=0;iter<xEffectsQueued.size();iter++)
		{
			if(xEffectsQueued[iter]->s_EffectName==s_EffectName)
			{				 
				return true;
			}

		}
		return false;
	}
	bool CEffectsManager::loadEffect(const std::string &s_EffectName)
	{
		return loadEffect(s_EffectName,"","");
	}


	/*
	*	Loads effect	
	*	- returns true if loaded ok
	*/
	bool CEffectsManager::loadEffect(const std::string &s_EffectName,const std::string &s_optFragmentShader, const std::string &s_optVertexShader)
	{
		Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_EffectName.c_str());
		if(s_EffectName.empty()==true)
		return false;


		//Create a new effect
		XEffect *e_New=new XEffect;

		std::string s_Vert;
		std::string s_Frag;

		
		//Load Vertex shader
		if(s_optVertexShader.empty()==true)
			s_Vert="xfx_Default";
		else
			s_Vert=s_optVertexShader;



		//Load Fragment shader
		if(s_optFragmentShader.empty()==true)
			s_Frag=s_EffectName;
		else 
			s_Frag=s_optFragmentShader;
		


		//Load and setup shader
		if(Game->ShadersManager()->loadShader(s_EffectName,s_Frag,s_Vert)==true)
		{
			X::Shader *Effect=NULL;
			Effect=Game->ShadersManager()->getShader(s_EffectName);
			if(Effect==NULL)return false;

			Effect->s_shaderName=s_EffectName;

			//Set the default uniform data
			Effect->TurnOn();
			Effect->SetInt(Effect->getUniform("t_FrameTexture"),0);
			Effect->SetInt(Effect->getUniform("t_FrameTextureOriginal"),1);
			Effect->SetInt(Effect->getUniform("t_FrameDepthTexture"),2);
			Effect->TurnOff();

		}
		else return false;
		 

		 

		e_New->s_FragFile=s_Frag;
		e_New->s_VertFile=s_Vert;
		e_New->s_EffectName=s_EffectName;



		//Bind the  __modificable__ frame texture to this FBO
		Game->FBOManager()->Create(s_EffectName,"",Game->getScreenSize(),0);		
		Game->FBOManager()->AttachTexture(s_EffectName,GL_COLOR_ATTACHMENT0,Game->getFrameTexture(),GL_TEXTURE_2D);
		Game->FBOManager()->Unbind(s_EffectName,"");

		//Push the effect onto the list
		xEffects.push_back(e_New);

		Game->Log()->write("<%s:%i %s> \"%s\" Loaded OK.",__FILE__,__LINE__,__FUNCTION__,s_EffectName.c_str());
		return true;
	}





	//Add effect to effects queue
	bool CEffectsManager::addEffect(const std::string &s_EffectName)
	{
		Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_EffectName.c_str());
		int iter;
		for(iter=0;iter<xEffects.size();iter++)
		{
			if(xEffects[iter]->s_EffectName==s_EffectName)
			{
				// Reset the timer
				xEffects[iter]->f_InternalTimer=0;

				xEffectsQueued.push_back(xEffects[iter]);
				return true;
			}

		}

		Game->Log()->write("<%s:%i %s> \"%s\" not found.",__FILE__,__LINE__,__FUNCTION__,s_EffectName.c_str());
		return false;
	}



	//Remove the effect from effects queue
	void CEffectsManager::removeEffect(const std::string &s_EffectName)
	{
		Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,s_EffectName.c_str());
		int iter;
		for(iter=0;iter<xEffectsQueued.size();iter++)
		{
			if(xEffectsQueued[iter]->s_EffectName==s_EffectName)
			{
				xEffectsQueued.erase(xEffectsQueued.begin()+iter);
				 
			}
			
		}
		 

		for(iter=0;iter<xEffectsQueued.size();iter++)
		{
	
				Game->Log()->write("<%s:%i %s> Still queued: \"%s\"",__FILE__,__LINE__,__FUNCTION__,xEffectsQueued[iter]->s_EffectName.c_str());
		}
	}


	int CEffectsManager::getEffectsQueued()
	{
		return xEffectsQueued.size();
	}
	 
	void CEffectsManager::removeQueuedEffects()
	{

		xEffectsQueued.clear();
	 
	}

	//Iterate through the 'queued' effects and render to engine's t_TextureFrame
	void CEffectsManager::renderEffects()
	{

		

		if(xEffectsQueued.size()>0)
		{
				
				X::Shader	*Effect=NULL;
				float p[2];
				int efCt=0;
				for(efCt=0;efCt<xEffectsQueued.size();efCt++)
				{

				
					glClearColor(0,0,0,1);
					glClear(GL_COLOR_BUFFER_BIT);	// Clear The Color buffer

					//Texture0 - is the effects modified texture of the frame
					glActiveTexture(XTEXTURE0);
					glClientActiveTextureARB(XTEXTURE0);							 
					glBindTexture(GL_TEXTURE_2D,Game->getFrameTexture());	

					//Texture1 - is the original not modificable texture
					glActiveTexture(XTEXTURE1);
					glClientActiveTextureARB(XTEXTURE1);		
					glBindTexture(GL_TEXTURE_2D,Game->getFrameTextureOriginal());	
 
					//Texture2 - is the original not modificable texture
					glActiveTexture(XTEXTURE2);
					glClientActiveTextureARB(XTEXTURE2);		
					glBindTexture(GL_TEXTURE_2D,Game->getFrameDepthTexture());	

					if(Game->FBOManager()->Bind(xEffectsQueued[efCt]->s_EffectName,"",-1)==true)
					{

				
						Effect=Game->ShadersManager()->getShader(xEffectsQueued[efCt]->s_EffectName);
						if(Effect!=NULL)
						{
							Effect->TurnOn();
							Effect->SetFloat(Effect->getUniform("f_Timer"),xEffectsQueued[efCt]->f_InternalTimer);

							glCallList(Game->getFrameRenderList());				
							Effect->TurnOff();
						}


						Game->FBOManager()->Unbind(xEffectsQueued[efCt]->s_EffectName,"");
				
					}
			 
					 
				}
			 
		}

	}

void CEffectsManager::Tick()
{
	int efCt=0;
	for(efCt=0;efCt<xEffectsQueued.size();efCt++)
	{
		if(xEffectsQueued[efCt]->f_InternalTimer<1.0)xEffectsQueued[efCt]->f_InternalTimer+=0.0001+Game->getDelta()/1000.0f;
	}
}
void CEffectsManager::AddRef()
{
        m_refCount++;
}

void CEffectsManager::Release()
{
        m_refCount--;
}
}