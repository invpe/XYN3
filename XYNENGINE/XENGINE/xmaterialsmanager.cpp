#include "xmain.h"

namespace X
{

	CMaterialsManager::CMaterialsManager(){}
	CMaterialsManager::~CMaterialsManager(){}


	/*
	*	Returns material if found
	*	[Input] a shortened version of the initial texture path
	*/
	tMaterial *CMaterialsManager::getMaterial(const std::string &sMaterialName)
	{
				 int iter;
				for(iter=0;iter<pMaterials.size();iter++)
				{

					if(pMaterials[iter]->sMaterialName==sMaterialName)
					{						 
						return pMaterials[iter];
					}

				}

				return NULL;

	}


	/*
	*	Takes in material definition and model transformation 
	*/
	bool CMaterialsManager::bindMaterial(tMaterial *material,const CMatrix4x4 &m_Model)
	{
		 
		return true;
	}


	void CMaterialsManager::unbindMaterial()
	{
		glUseProgramObjectARB(0);	
	}

	void CMaterialsManager::reloadMaterials()
	{
		Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);
		int iter;
		for(iter=0;iter<pMaterials.size();iter++)
		{
			std::string matName;
			matName=pMaterials[iter]->sMaterialName;
			
			removeMaterial(matName);
			loadMaterial(matName);
		}
	}

	void CMaterialsManager::removeMaterial(const std::string &sMaterialName)
	{
		Game->Log()->write("<%s:%i %s> %s",__FILE__,__LINE__,__FUNCTION__,sMaterialName.c_str());
		tMaterial *material;
		material=getMaterial(sMaterialName);

		if(material!=NULL)
		{

			//Remove the texture first
			int tex;
			for(tex=0;tex<material->Textures.size();tex++)
			{				
				Game->TexturesManager()->removeTexture(material->Textures[tex]->s_TextureName);
			}


			//We can't remove shader as other materials can share it
			//Game->ShadersManager()->destroyShader(material->sMaterialShader);
			

			//Remove the material from the list
			int iter;
			for(iter=0;iter<pMaterials.size();iter++)
			{
				if(pMaterials[iter]->sMaterialName==sMaterialName)
				{
					pMaterials.erase(pMaterials.begin()+iter);					
				}
			}

			Game->Log()->write("<%s:%i %s> %s removed, Materials managed: %i",__FILE__,__LINE__,__FUNCTION__,sMaterialName.c_str(),pMaterials.size());
			
		}

			
	}

	/*
	*	Loads material data and corresponding textures
	*	[Input] a shortened version of the initial texture path
	*/
	bool CMaterialsManager::loadMaterial(const std::string &sMaterialName)
	{
	 	Game->Log()->write("<%s:%i %s> \"%s\"",__FILE__,__LINE__,__FUNCTION__,sMaterialName.c_str());

		//Check if filename is not empty
		if(sMaterialName.empty()==true)
		{
			Game->Log()->write("<%s:%i %s> Error, name empty - \"%s\"",__FILE__,__LINE__,__FUNCTION__,sMaterialName.c_str());
			return false;
		}
		 
		//Prevent from loading the same material X times
		if(getMaterial(sMaterialName)!=NULL)
		{			
			return true;
		}

		//Convert to Xynapse xmat filename
		std::string s_MaterialFileName;
		s_MaterialFileName="data/materials/"+sMaterialName+".xmat";

		ifstream fin;
		fin.open(s_MaterialFileName.c_str());
		if(!fin)
		{
	 			Game->Log()->write("<%s:%i %s> Warrning, \"%s\" not found - creating default material.",__FILE__,__LINE__,__FUNCTION__,s_MaterialFileName.c_str());

	 			fin.open("data/materials/default.xmat");
				if(!fin)
				{
						Game->Log()->write("<%s:%i %s> Error, default.xmat not found!",__FILE__,__LINE__,__FUNCTION__);
						return false;
				}
				 
					Game->Log()->write("<%s:%i %s> Parsing default.xmat",__FILE__,__LINE__,__FUNCTION__);
 	 
		}
		 

			   


		
		//Create our new material object
		tMaterial *pNewMaterial=new tMaterial;
		pNewMaterial->sMaterialName=sMaterialName;

		//Define the default shader for this material
		pNewMaterial->sMaterialShader="basic";



		 				//Parsing now
						string s_Line = "";
						char s_Temp[256];int i_Temp;
						while(getline(fin, s_Line))
						{

							if(!strstr(s_Line.c_str(),"//")&&strlen(s_Line.c_str())>3)
							{


								if(strstr(s_Line.c_str(),"materialAmbient"))sscanf(s_Line.c_str(),"materialAmbient = %f%f%f%f",&pNewMaterial->vAmbient.x,&pNewMaterial->vAmbient.y,&pNewMaterial->vAmbient.z,&pNewMaterial->vAmbient.w);
								if(strstr(s_Line.c_str(),"materialDiffuse"))sscanf(s_Line.c_str(),"materialDiffuse = %f%f%f%f",&pNewMaterial->vDiffuse.x,&pNewMaterial->vDiffuse.y,&pNewMaterial->vDiffuse.z,&pNewMaterial->vDiffuse.w);
								if(strstr(s_Line.c_str(),"materialSpecular"))sscanf(s_Line.c_str(),"materialSpecular = %f%f%f%f",&pNewMaterial->vSpecular.x,&pNewMaterial->vSpecular.y,&pNewMaterial->vSpecular.z,&pNewMaterial->vSpecular.w);
								if(strstr(s_Line.c_str(),"materialShininess"))sscanf(s_Line.c_str(),"materialShininess = %f",&pNewMaterial->fShininess);
								if(strstr(s_Line.c_str(),"lighting")){sscanf(s_Line.c_str(),"lighting = %i",&i_Temp);pNewMaterial->bCalculateLighting=i_Temp;}
								if(strstr(s_Line.c_str(),"shadows")){sscanf(s_Line.c_str(),"shadows = %i",&i_Temp);pNewMaterial->bCalculateShadows=i_Temp;}

								if(strstr(s_Line.c_str(),"shader"))
								{
										sscanf(s_Line.c_str(),"shader = %s",s_Temp);
										pNewMaterial->sMaterialShader=s_Temp;
										
								}

								//Create color texture
								if(strstr(s_Line.c_str(),"texture"))
								{
									sscanf(s_Line.c_str(),"texture = %i %s",&i_Temp,s_Temp);

									tMaterialTexture *t_newMaterialTexture=new tMaterialTexture;
									t_newMaterialTexture->s_FullTexturePath=s_Temp;
									t_newMaterialTexture->i_ActiveTextureBindNumber=i_Temp;

									//Get rid of the path, return the texture name in TexturesManager convention
	 								char *token=NULL;
									token=strtok( s_Temp ,"/");
									while(token != NULL)
									{
		 								t_newMaterialTexture->s_TextureName=token;
										token=strtok(NULL,"/");
									}


									//Now we have all required to manage this material.
									if(Game->TexturesManager()->loadTexture(t_newMaterialTexture->s_FullTexturePath)==false)
									{
										return false;
									}
									
									Game->Log()->write("<%s:%i %s> Texture reference name \"%s\" added to this material, bound as %i",__FILE__,__LINE__,__FUNCTION__,t_newMaterialTexture->s_TextureName.c_str(),t_newMaterialTexture->i_ActiveTextureBindNumber);
									pNewMaterial->Textures.push_back(t_newMaterialTexture);



								}
							}
			
						}

						// Close our file
						fin.close();		
		  


		//Load material shader to the shader's manager list
		Game->ShadersManager()->loadShader(pNewMaterial->sMaterialShader,pNewMaterial->sMaterialShader,pNewMaterial->sMaterialShader);
		if(Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)!=NULL)
		{

			Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->TurnOn();
			Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->SetInt(Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->getUniform("colorMap"),6);
			Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->SetInt(Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->getUniform("normalMap"),7);
			Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->SetInt(Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->getUniform("specularMap"),8);
			//Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->SetInt(Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->getUniform("ssaoMap"),9);
			Game->ShadersManager()->getShader(pNewMaterial->sMaterialShader)->TurnOff();
		}
		else return false;


		Game->Log()->write("<%s:%i %s> Material loaded.",__FILE__,__LINE__,__FUNCTION__);



		//Push back newly created material
		pMaterials.push_back(pNewMaterial);
		return true;
	}

};