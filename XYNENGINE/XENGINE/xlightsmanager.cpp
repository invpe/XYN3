		//directional light -- source is infinitely away
		//GLfloat light_position[] = { 1.0, 1.0, 1.0, **0.0** };
		//The fourth component of light_position is 0.0 which signifies the light is directional light.

		//positional light -- source at a finite distance
		//GLfloat light_position[] = { 5.0, 10.0, 2.0, **1.0** };
		//The location is transformed by the model view matrix and stored in the eye coordinate system (i.e. relative to the eye )


#include "xmain.h"
 
namespace X
{



		/*
		*		
		*	QSORT compare function, returns the brighter light from two.	
		*
		*/
		 int lightBrightnessCompare(const LightData &a, const LightData &b)
		 {
		 
		   if(a.distance<b.distance)return 1;
		   else return 0;

		 }


	/*This creates 4 (LIGHTS_MAX_DYNAMIC) cubeshadowmaps for lights.*/
	void CLightsManager::initializeMaps()
	{

	//Generate LIGHTS_MAX_DYNAMIC cubemaps
		for(int a=0;a<LIGHTS_MAX_DYNAMIC;a++)
		{
			glGenTextures(1, &tDepthCubeMap[a]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tDepthCubeMap[a]);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			 
			for (int face = 0; face < 6; face++) 
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT   , 512, 512, 0,  GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0); 
		}

	}



	CLightsManager::CLightsManager()
	{
		
		 

	}

	CLightsManager::~CLightsManager(){}


	void CLightsManager::AddRef()
{
        m_refCount++;
}

void CLightsManager::Release()
{
        m_refCount--;
}

	/*
	* Checks if a light is bound to any object/bone and updates it's position per tick
	*/
	void CLightsManager::processLights()
	{


		int lCt=0;
		if(Lights.size()>0)
		for(lCt=0;lCt<Lights.size();lCt++)
		{

			/*
			*	Update the light shadowmapping matrices
			*/
			if(Lights[lCt]->castShadows==true)
			{

				Lights[lCt]->m_LightModel.Clear();
				Lights[lCt]->m_LightModel.Translate(-Lights[lCt]->getPosition().x,-Lights[lCt]->getPosition().y,-Lights[lCt]->getPosition().z);
				for(int a=0;a<6;a++)
				{
					Lights[lCt]->m_LightModelViewProjection[a]=Lights[lCt]->m_LightProjection*Lights[lCt]->m_LightView[a]*Lights[lCt]->m_LightModel;
				}

			}
  

			/*
			*	Update the light MODEL transformation matrix - the actual position of the light
			*/
			if(Lights[lCt]->boundActorName.empty()==false)
			{		
				 
				//Bind to actor and his bone position
				if(Lights[lCt]->boundBoneName.empty()==false)
				{

					//Calculate the exact light position for bound bone.
					X::CActor *a_ActorBound;
					a_ActorBound=Game->ActorsManager()->getActor(Lights[lCt]->boundActorName);
					if(a_ActorBound!=NULL)
					{
						XModel::tBoneData *b_Bone;
						b_Bone=a_ActorBound->m_pModel->getBone(Lights[lCt]->boundBoneName.c_str());
						Lights[lCt]->m_Model=a_ActorBound->m_Model*b_Bone->getPositionMatrix();
					} 
		 
				}
		 

			}

			 


		}

	}





	/*
	*		passLightsToShader(X::Shader	&Shader)
	*		arguments - Shader
	*		synopsis  - will pass lightType / (defined structure) to the pointed shader for future per-shader work
	*/
	void CLightsManager::passLightsToShader(X::Shader *Shader)
	{
		int lct=0;
		char uniformName[128];
		int lightsCount=getLightsCount();

		//Limit US to max lights only
		if(lightsCount>LIGHTS_MAX_DYNAMIC)lightsCount=LIGHTS_MAX_DYNAMIC;


		 	for(lct=0;lct<lightsCount;lct++)
			{
			 
				
				sprintf(uniformName,"Light[%i].type",lct);
				Shader->SetInt(Shader->getUniform(uniformName), getLight(lct)->lightType); 

				sprintf(uniformName,"Light[%i].m_Model",lct);
				Shader->SetMatrix4x4(Shader->getUniform(uniformName),getLight(lct)->m_LightModel); 

				sprintf(uniformName,"Light[%i].m_Projection",lct);
				Shader->SetMatrix4x4(Shader->getUniform(uniformName),getLight(lct)->m_LightProjection); 
				
				sprintf(uniformName,"Light[%i].radius",lct);
				Shader->SetFloat(Shader->getUniform(uniformName),getLight(lct)->radius ); 

				sprintf(uniformName,"Light[%i].castShadow",lct);
				Shader->SetInt(Shader->getUniform(uniformName),getLight(lct)->castShadows ); 

				sprintf(uniformName,"Light[%i].ambient",lct);
				Shader->SetVector4(Shader->getUniform(uniformName),getLight(lct)->ambient);

				sprintf(uniformName,"Light[%i].diffuse",lct);
				Shader->SetVector4(Shader->getUniform(uniformName),getLight(lct)->diffuse);

				sprintf(uniformName,"Light[%i].specular",lct);
				Shader->SetVector4(Shader->getUniform(uniformName),getLight(lct)->diffuse);

				sprintf(uniformName,"Light[%i].position",lct);
				Shader->SetVector3(Shader->getUniform(uniformName),getLight(lct)->getPosition()); 

				if(getLight(lct)->castShadows==true)
				{
					sprintf(uniformName,"Light[%i].shadowMap",lct);
					Shader->SetInt(Shader->getUniform(uniformName),lct ); 
				}


				 

			}
				
		 

	}

 void CLightsManager::setCastShadows(int lightNumber,bool bCastShadows)
 {
	  if(lightNumber<Lights.size()) Lights[lightNumber]->castShadows=bCastShadows;
 }

 void CLightsManager::setLightPosition(int lightNumber,const CVector3 &newPosition)
 {
	  if(lightNumber<Lights.size()) Lights[lightNumber]->m_Model.Translate(newPosition.x,newPosition.y,newPosition.z);
 }
 void CLightsManager::setLightRadius(int lightNumber,float radius)
 {
	 if(lightNumber<Lights.size()) Lights[lightNumber]->radius=radius;

 }
 int CLightsManager::createLight(const CVector3 &position,const CVector3 &color_ambient,const CVector3 &color_diffuse,LightType l_Type,float radius,bool cast_shadows)
 {
	 Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);

	LightData *l_New=new LightData(position,color_ambient,color_diffuse,l_Type,radius,cast_shadows);
	
	Lights.push_back(l_New);
	 
	return Lights.size()-1;
		
 }

 
	/*
	*	deleteLight(int l_id)
	*	arguments	-	 light id
	*	synopsis	-	 removes selected light
	*/
	bool CLightsManager::deleteLight(int l_id)
	{
		if(l_id<Lights.size())
		{
			

			Lights[l_id]->~LightData();

			Lights.erase(Lights.begin()+l_id);
 			return true;

		}

		return false;
	}


	/*
	*	getLight(int l_id)
	*	arguments	-	 light id
	*	synopsis	-	 returns selected light structure
	*/
	LightData *CLightsManager::getLight(int l_id)
	{

		if(l_id<Lights.size())
		{
			return Lights[l_id];
		}

		return NULL;
		
	}

 
	 

	/*
	*	calculateLighting()
	*	arguments			-		position
	*	returns				-		visible lights per position, this number goes to shader that iterates
	*						-		we take into consideration 4 lights to be calculated.
	*						-		1 light for player (candle,...)
	*						-		3 lights for room
	*/
	int CLightsManager::calculateLighting(const CVector3 &rPosition,X::CCamera *Camera,X::CFrustum	*Frustum)
	{

	int it;
	int lightsCount;

	
	lightsCount=Lights.size();
	
	//Sort only if there are more than 4 lights on the scene
	if(lightsCount>LIGHTS_MAX_DYNAMIC)
	{
		for(int i=0; i<lightsCount; i++)
 		{
		  
			//If light is invisible in frustum, set brightness to far far away from those 'locals'
			//so we don't considerate this visible at all.
			if(!Frustum->CubeInFrustum(Lights[i]->getPosition().x,Lights[i]->getPosition().y,Lights[i]->getPosition().z,Lights[i]->radius))
 				Lights[i]->distance=10000;
			else
				Lights[i]->distance=mathGetfdistance(rPosition.x,rPosition.z,Lights[i]->getPosition().x,Lights[i]->getPosition().z);
 		}


		//Sort the lights by their brightness calculated above
	 	//std::sort(Lights.begin(),Lights.end(),lightBrightnessCompare);

	}
		//Limit US to LIGHTS_MAX_DYNAMIC only.
		if(lightsCount>LIGHTS_MAX_DYNAMIC)lightsCount=LIGHTS_MAX_DYNAMIC;
 
		return it;

	}

	/*
	*	getLightsCount()
	*	arguments	-	
	*	synopsis	- returns visible lights count
	*/
	int CLightsManager::getLightsCount()
	{
		return Lights.size();
	}

	void CLightsManager::renderLightSources()
	{


		int it;
		for(it=0;it<Lights.size();it++)
		{

			/*glPushMatrix();//Enter model matrix, push View matrix
			glMultMatrixf(Lights[it]->m_Model);//View * Model matrix						 
			glColor3f(Lights[it]->diffuse[0],Lights[it]->diffuse[1],Lights[it]->diffuse[2]);
			glutWireSphere(Lights[it]->radius,6,6);
			
			glutWireSphere(5,6,6);
			glPopMatrix();*/

		}


	}


	int CLightsManager::XS_createLight(const CVector3 &position,const CVector3 &color_ambient,const CVector3 &color_diffuse,LightType l_Type,float radius,bool cast_shadows)
	{					
		Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);

 						return createLight(position,color_ambient,color_diffuse,l_Type,radius,cast_shadows);
						  
	}

bool CLightsManager::XS_bindLightToActorBone(int lightId,const std::string &actorName,const std::string &boneName)
{
	Game->Log()->write("<%s:%i %s> %s %s ",__FILE__,__LINE__,__FUNCTION__,actorName.c_str(),boneName.c_str());

/*	 CActorDefinition *ac=Game->ActorsManager()->getActor(actorName);
	 if(!ac){	Game->Log()->write("<%s:%i %s> Error, \"%s\" Actor not found",__FILE__,__LINE__,__FUNCTION__,actorName.c_str());		return false;}

		if(ac->pModel->getBone(boneName.c_str())==NULL)
		{
			Game->Log()->write("<%s:%i %s> Error, \"%s\" Bone not found",__FILE__,__LINE__,__FUNCTION__,boneName.c_str());		return false;
		}*/

	 
	return getLight(lightId)->bindTo(actorName,boneName);



}

bool CLightsManager::XS_deleteLight(int id)
{
	Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);
	return deleteLight(id);
	 
} 


	/*
	*		PER LIGHT METHODS
	*/

	  LightData::LightData(CVector3 _position, CVector3 _ambient, CVector3 _diffuse, LightType Type, float _radius,bool _castShadows)
	  {
 
		   Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);
		   m_Model.Clear();
		   m_Model.Translate(_position.x,_position.y,_position.z);

		  ambient.x=_ambient.x;
		  ambient.y=_ambient.y;
		  ambient.z=_ambient.z;
		  ambient.w=1;

		  diffuse.x=_diffuse.x;
		  diffuse.y=_diffuse.y;
		  diffuse.z=_diffuse.z;
		  diffuse.w=1;
		  
		  lightType=Type;

		  radius=_radius*2;

		 castShadows=_castShadows;


			m_LightView[0].createViewMatrix(CVector3(0,0,0),CVector3(1,0,0),	CVector3(0,-1,0));// +X
			m_LightView[1].createViewMatrix(CVector3(0,0,0),CVector3(-1,0,0),	CVector3(0,-1,0));// -X
			m_LightView[2].createViewMatrix(CVector3(0,0,0),CVector3(0,1,0),	CVector3(0,0,1));// +Y
			m_LightView[3].createViewMatrix(CVector3(0,0,0),CVector3(0,-1,0),	CVector3(0,0,-1));// -Y
			m_LightView[4].createViewMatrix(CVector3(0,0,0),CVector3(0,0,1),	CVector3(0,-1,0));// +Z
			m_LightView[5].createViewMatrix(CVector3(0,0,0),CVector3(0,0,-1),	CVector3(0,-1,0));// -Z

			m_LightProjection.createProjectionMatrix(90,1,0.01,radius);


	 }

	LightData::~LightData()
	{
			Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);
			 

	}
	
 

	 
	 /*
	 *	Bind light to Actor/Bone/Actor+bone
	 */
	  bool LightData::bindTo(std::string s_ActorName,std::string s_BoneName)
	  {
		 
		 boundActorName=s_ActorName;
		 boundBoneName=s_BoneName;

		 return true;
	  }

	  void LightData::Unbind()
	  {		
		  boundBoneName.clear();
		  boundActorName.clear();
	  }




	  /*
	  * Transform world space light position to camera space
	  */
	  void LightData::Process(const float m_View[16])
	  {

			position_cs[0]=
				m_View[ 0] * getPosition().x +
				m_View[ 4] * getPosition().y +
				m_View[ 8] * getPosition().z +
				m_View[12];
			position_cs[1] =
				m_View[ 1] * getPosition().x +
				m_View[ 5] * getPosition().y +
	
				m_View[ 9] * getPosition().z +
				m_View[13];
			position_cs[2] =
				m_View[ 2] * getPosition().x +
				m_View[ 6] * getPosition().y +
				m_View[10] * getPosition().z +
				m_View[14];

	  }



	  CVector3 LightData::getPosition()
	  {
		 return CVector3(m_Model.GetColumn(3).x,m_Model.GetColumn(3).y,m_Model.GetColumn(3).z);
	  }





}





/*
namespace X
{

 
 int lightBrightnessCompare(const void *a, const void *b)
 {
   lightdata *ld1 = (lightdata *) a;
   lightdata *ld2 = (lightdata *) b;
   GLfloat diff;
 
  The brighter lights get sorted close to top of the list.
   diff = ld2->brightness - ld1->brightness;
 
   if (diff > 0)
     return 1;
   if (diff < 0)
     return -1;
   return 0;
 }
 
 char lighting::createlight(float x, float y, float z,
 		    float Dr, float Dg, float Db,
 		    float Ar, float Ag, float Ab,
 		    float cA, float lA, float qA)
 {
 	lightP[num_lights].pos[0] = x;
 	lightP[num_lights].pos[1] = y;
 	lightP[num_lights].pos[2] = z;
 	lightP[num_lights].pos[3] = 1.0f;
 
 	lightP[num_lights].diffuse[0] = Dr;
 	lightP[num_lights].diffuse[1] = Dg;
 	lightP[num_lights].diffuse[2] = Db;
 	lightP[num_lights].diffuse[3] = 1.0f;
 
 	lightP[num_lights].ambient[0] = Ar;
 	lightP[num_lights].ambient[1] = Ag;
 	lightP[num_lights].ambient[2] = Ab;
 	lightP[num_lights].ambient[3] = 1.0f;
 
 	lightP[num_lights].constantAttenuation = cA;
 	lightP[num_lights].linearAttenuation = lA;
 	lightP[num_lights].quadraticAttenuation = qA;
 

 	num_lights++;
 	return (1);
 }
 
 void lighting::updateLight(int id,CVector3 position)
 {

lightP[id].pos[0]=position.x;
lightP[id].pos[1]=position.y;
lightP[id].pos[2]=position.z;

 }
 int lighting::dolighting(CVector3 position,X::Camera *Camera,X::CFrustum	*Frustum)
 {
 
 
 // Calculate each lights brightness based on it's position, the object position and the eye position.
 	for(int i=0; i<num_lights; i++)
 	{
 		float dx = lightP[i].pos[0] - position.x;
 		float dy = lightP[i].pos[1] - position.y;
 		float dz = lightP[i].pos[2] - position.z;
 
 		float quadraticAttenuation = dx * dx + dy * dy + dz * dz;
 		//lightP[i].brightness = -quadraticAttenuation;
 		
 		// -- DETERMINE LIGHT CONTRIBUTION -- // 
 		GLfloat ex, ey, ez;
 		GLfloat nx, ny, nz;
 		GLfloat distance;
 		GLfloat diffuseReflection;
 			  
 		// Determine eye point location (remember we can rotate by angle).
 		ex = Camera()->Position().x;
 		ey = Camera()->Position().y;
 		ez = Camera()->Position().z;
 			  
 		// Calculated normalized object to eye position direction (nx,ny,nz).
 		nx = (ex - position.x);
 		ny = (ey - position.y);
 		nz = (ez - position.z);
 		distance = sqrt(nx * nx + ny * ny + nz * nz);
 		nx = nx / distance;
 		ny = ny / distance;
 		nz = nz / distance;
 		  
 		// True distance needed, take square root.
 		distance = sqrt(quadraticAttenuation);
 			  
 		// Calculate normalized object to light position direction (dx,dy,dz)
 		dx = dx / distance;
 		dy = dy / distance;
 		dz = dz / distance;
 			  
 		diffuseReflection = nx * dx + ny * dy + nz * dz;
 		lightP[i].brightness = diffuseReflection / quadraticAttenuation;  
 	}
 	// -- SORT LIGHTS, AND ASSIGN TO HARDWARE -- //
 	qsort(lightP, num_lights, sizeof(lightP[0]), lightBrightnessCompare);  
 
	
 	for(int j=0; j<3; j++)
 	{

		//If light not in frustum
  		 if(!Frustum->PointInFrustum(lightP[j].pos[0], lightP[j].pos[1], lightP[j].pos[2]))
 		{
		
			j++;
		 	 
 		} 
		

 	
		
 		glLightfv(GL_LIGHT0+j, GL_POSITION, lightP[j].pos);
 		glLightfv(GL_LIGHT0+j, GL_DIFFUSE,  lightP[j].diffuse);
 		glLightfv(GL_LIGHT0+j, GL_AMBIENT,  lightP[j].ambient);
 		
		 		glLightf(GL_LIGHT0+j, GL_CONSTANT_ATTENUATION, lightP[j].constantAttenuation);
			glLightf(GL_LIGHT0+j, GL_LINEAR_ATTENUATION, lightP[j].linearAttenuation);
 			glLightf(GL_LIGHT0+j, GL_QUADRATIC_ATTENUATION, lightP[j].quadraticAttenuation);

 	
		 
      
 		
 	} 

   
		for(int j=0; j<num_lights; j++)
 		{
	
 			glLightfv(GL_LIGHT0+j, GL_POSITION, lightP[j].pos);
 			glLightfv(GL_LIGHT0+j, GL_DIFFUSE,  lightP[j].diffuse);
 			glLightfv(GL_LIGHT0+j, GL_AMBIENT,  lightP[j].ambient);

	 		glLightf(GL_LIGHT0+j, GL_CONSTANT_ATTENUATION, lightP[j].constantAttenuation);
			glLightf(GL_LIGHT0+j, GL_LINEAR_ATTENUATION, lightP[j].linearAttenuation);
 			glLightf(GL_LIGHT0+j, GL_QUADRATIC_ATTENUATION, lightP[j].quadraticAttenuation);

 
		}

 


	return NULL;
 }

 lighting::lighting()
 {
	 num_lights=0;
 }
 lighting::~lighting(){}

 }
 */