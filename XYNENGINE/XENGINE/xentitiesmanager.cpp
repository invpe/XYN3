#include "xmain.h"


namespace X
{
CEntity &CEntity::operator=(const CEntity &other)
{
		if (this != &other) // protect against invalid self-assignment
        {
		
		   this->angle=other.angle;
		   this->bindable=other.bindable;
		   this->castshadows=other.castshadows;
		   this->classname=other.classname;
		   this->color=other.color;
		   this->entity_name=other.entity_name;
		   this->fov=other.fov;
		   this->model_name=other.model_name;
		   this->origin=other.origin;
		   this->radius=other.radius;
		   this->switchable=other.switchable;
		   this->weight=other.weight;
		   this->targetname=other.targetname;
		   this->target=other.target;
		   this->time=other.time;
		   this->lookat=other.lookat;
		   this->bCharacter=other.bCharacter;

		}
		// Return a reference to this object
		return *this;
}
CEntity::CEntity()
{
		Reset(); 	
		
}
void CEntity::Reset()
{
			targetname.clear();
			classname.clear();
			model_name.clear();
			target.clear();
			targetname.clear();
			entity_name.clear();
			lookat.clear();
		 	radius=0;
			weight=0;
			fov=0;
			bindable=false;
			switchable=false;
			castshadows=false;
			bCharacter=false;
			angle=CVector3(0,0,0);
			origin=CVector3(0,0,0);
			color=CVector3(0,0,0);
			time=0;

}

CEntity::~CEntity()
{
}











 
CEntity *CEntitiesManager::getEntityName(const std::string &rEntityName)
{
	int it;
	for(it=0;it<pMapEntities.size();it++)
	{
	
		if(pMapEntities[it]->entity_name==rEntityName)
		{				 
			 return  &*pMapEntities[it];			 
		}
	}

	return NULL;
}

 
CEntity *CEntitiesManager::getEntityClass(const std::string &rClassName)
{
	 
	int it;
	for(it=0;it<pMapEntities.size();it++)
	{
	
	
		if(pMapEntities[it]->classname==rClassName)
		{
			return  &*pMapEntities[it];			  
		}
	}

	return NULL;
}


CEntity *CEntitiesManager::getEntityTargetName(const std::string &rTargetName)
{
	int it;
	for(it=0;it<pMapEntities.size();it++)
	{
	
	
		if(pMapEntities[it]->targetname==rTargetName)
		{
			return  &*pMapEntities[it];			  
		}
	}

	return NULL;
}

CEntity *CEntitiesManager::getEntityTarget(const std::string &rTarget)
{
	int it;
	for(it=0;it<pMapEntities.size();it++)
	{
	
	
		if(pMapEntities[it]->target==rTarget)
		{
			return  &*pMapEntities[it];			  
		}
	}

	return NULL;
}


CEntitiesManager::CEntitiesManager()
{
}

CEntitiesManager::~CEntitiesManager()
{
}


bool CEntitiesManager::parseEntities(char *pEntities)
{
	Game->Log()->write("<%s:%i %s> Parsing\n %s",__FILE__,__LINE__,__FUNCTION__,pEntities);
	
	CEntity mNewEntity;

	char *token=NULL;
	token=strtok(pEntities,"{"); 
	while(token != NULL)
	{
		
		
		 //Within the entity block
		if(!strstr(token,"{")&&!strstr(token,"}"))
		{


			//Actual data/models name
			if(strstr(token,"model_name"))
			{
				char modelName[128];
				sscanf(token,"\"model_name\" \"%s\"",modelName);
				modelName[strlen(modelName)-1]='\0';
				mNewEntity.model_name=modelName;

			}



			//Unique engine entity name
			if(strstr(token,"entity_name"))									
			{	
				char entityName[128];
				sscanf(token,"\"entity_name\" \"%s\"",entityName);
				entityName[strlen(entityName)-1]='\0';
				mNewEntity.entity_name=entityName;
			}

			if(strstr(token,"targetname"))
			{
				char targetName[128];
				sscanf(token,"\"targetname\" \"%s\"",targetName);
				targetName[strlen(targetName)-1]='\0';
				mNewEntity.targetname=targetName;
			}

			if(strstr(token,"target") && !strstr(token,"targetname"))
			{
				char target[128];
				sscanf(token,"\"target\" \"%s\"",target);
				target[strlen(target)-1]='\0';
				mNewEntity.target=target;
			}
			if(strstr(token,"LookAt"))
			{
				char LookAt[128];
				sscanf(token,"\"LookAt\" \"%s\"",LookAt);
				LookAt[strlen(LookAt)-1]='\0';
				mNewEntity.lookat=LookAt;
			}

			 
			if(strstr(token,"time"))												{sscanf(token,"\"time\" \"%f\"",&mNewEntity.time);			}
		  	if(strstr(token,"fov"))												{sscanf(token,"\"fov\" \"%i\"",&mNewEntity.fov);			}
			if(strstr(token,"castshadows"))										{int bol;	sscanf(token,"\"castshadows\" \"%i\"",&bol);mNewEntity.castshadows=bol;	}
			if(strstr(token,"bCharacter"))										{int bol;	sscanf(token,"\"bCharacter\" \"%i\"",&bol);mNewEntity.bCharacter=bol;	}
			if(strstr(token,"switchable"))										{int bol;	sscanf(token,"\"switchable\" \"%i\"",&bol);mNewEntity.switchable=bol;	}
			if(strstr(token,"bindable"))										{int bol;	sscanf(token,"\"bindable\" \"%i\"",&bol);mNewEntity.bindable=bol;	}
			if(strstr(token,"origin"))											{	sscanf(token,"\"origin\" \"%f%f%f\"",&mNewEntity.origin.x,&mNewEntity.origin.z,&mNewEntity.origin.y);	}
			if(strstr(token,"angles"))											{	sscanf(token,"\"angles\" \"%f%f%f\"",&mNewEntity.angle.z,&mNewEntity.angle.y,&mNewEntity.angle.x);	}
		 	if(!strstr(token,"classname")  && strstr(token,"light"))			{	sscanf(token,"\"light\" \"%f\"",&mNewEntity.radius);	}
			if(strstr(token,"castshadows"))										{int bol;	sscanf(token,"\"castshadows\" \"%i\"",&bol);mNewEntity.castshadows=bol;	}
			if(strstr(token,"_color"))											{	sscanf(token,"\"_color\" \"%f %f %f\"",&mNewEntity.color.x,&mNewEntity.color.y,&mNewEntity.color.z);	}
			if(strstr(token,"weight"))											{	sscanf(token,"\"weight\" \"%f\"",&mNewEntity.weight);	}
			

			//Classname - does not impact
			if(strstr(token,"classname"))
			{
				char className[128];
				sscanf(token,"\"classname\" \"%s\"",className);
				className[strlen(className)-1]='\0';				 
				mNewEntity.classname=className;
			}

	  
		}


		//End of entity block
		else if(!strstr(token,"{") && strstr(token,"}"))
		{

			if(mNewEntity.classname.empty()==false)
			{
				mNewEntity.origin.z*=-1;		


				CEntity *pEntity=new CEntity;
				*pEntity=mNewEntity;				
				pMapEntities.push_back(pEntity);		
				

				//Reset the Entity, so it wont overwrite the next one
				mNewEntity.Reset();
			}

		}


		token=strtok(NULL,"\n");
	}
	

	for(int a=0;a<pMapEntities.size();a++)
	{
		 Game->Log()->write("<%s:%i %s> Entity Class: %s added.",__FILE__,__LINE__,__FUNCTION__,pMapEntities[a]->classname.c_str());
	}

	 
	/*
			 Game->Log()->write("<%s:%i %s> Seeding entities... ",__FILE__,__LINE__,__FUNCTION__);
  

			 list<CEntity>::iterator iter;
			 for(iter=pMapEntities.begin();iter!=pMapEntities.end();iter++)
			 {

				  Game->Log()->write("<%s:%i %s> Seeding %s, rot=%f",__FILE__,__LINE__,__FUNCTION__,iter->class_name.c_str(),iter->angle.y);
				 if(iter->class_name=="light")
				 {
					 Game->LightsManager()->createLight(iter->origin,CVector3(0,0,0),iter->color,X::LightType::XLIGHT_OMNI,iter->radius,iter->castshadows);
				 }

				 //If model name exists - we load actor here
				 if(iter->model_name.empty()==false)
				 {

					 Game->Log()->write("<%s:%i %s> \t %s/%s",__FILE__,__LINE__,__FUNCTION__,iter->entity_name.c_str(),iter->model_name.c_str());
					 if(Game->ActorsManager()->createActor(iter->entity_name,iter->model_name,iter->origin,X::ACTORTYPE_OBJECT)==false)
					 {						 
						 Game->Log()->write("<%s:%i %s> Error, cannot create actor! ",__FILE__,__LINE__,__FUNCTION__);
						 return false;
					 }
					 
					 //Set object properties here too
					 if( Game->ActorsManager()->getActor(iter->entity_name)!=NULL)
					 {
						 Game->ActorsManager()->getActor(iter->entity_name)->b_castShadows=iter->castshadows;
						 Game->ActorsManager()->getActor(iter->entity_name)->setMass(iter->weight);					 						 
						 Game->ActorsManager()->getActor(iter->entity_name)->Rotate(CVector3(iter->angle.x,iter->angle.y,iter->angle.z));
						 X::tActorFlags a_Flags;
						 
						 a_Flags.bBindable=iter->bindable;
						 a_Flags.bSwitchable=iter->switchable;						 
						 Game->ActorsManager()->getActor(iter->entity_name)->setFlags(a_Flags);
					 }
 

				 }


			 }

 
			 //Find and create player
			 Game->Log()->write("<%s:%i %s> Seeding Player... ",__FILE__,__LINE__,__FUNCTION__);			 
			 
			 CEntity *a_Player=getEntityClass("start");
			 if(a_Player!=NULL)
			 {
				 
					 
					 if(Game->ActorsManager()->createActor("Player","ag",a_Player->origin,ACTORTYPE_PLAYER)==true)
					 {
						  
							Game->ActorsManager()->getActor("Player")->fRotation=a_Player->angle.y;
							Game->ActorsManager()->getActor("Player")->b_castShadows=true;
						 
					 }
					 else
					 {
						 Game->Log()->write("<%s:%i %s> error... ",__FILE__,__LINE__,__FUNCTION__);
						 return false;
					 }
		 
			 }
			 else
			 {
				 Game->Log()->write("<%s:%i %s> \"start\" not found. ",__FILE__,__LINE__,__FUNCTION__);
				  
			 }*/
			 
 

	Game->Log()->write("<%s:%i %s> Parsing finished.",__FILE__,__LINE__,__FUNCTION__);
	return true;
}


void CEntitiesManager::seedEntities()
{
	 
	for(int a=0;a<pMapEntities.size();a++)
	{


				 if(pMapEntities[a]->classname=="light")
				 {
					 Game->LightsManager()->createLight(pMapEntities[a]->origin,CVector3(0,0,0),pMapEntities[a]->color,X::LightType::XLIGHT_OMNI,pMapEntities[a]->radius,pMapEntities[a]->castshadows);
				 }

				 if(pMapEntities[a]->model_name.empty()==false)
				 {
					 if(Game->ActorsManager()->CreateActor(pMapEntities[a])==true)
					 {
						 
						Game->ActorsManager()->getActor(pMapEntities[a]->entity_name)->Rotate(CVector3(pMapEntities[a]->angle.x,pMapEntities[a]->angle.y,pMapEntities[a]->angle.z));

					 }
				 }
				 
   }
	 
}



bool CEntitiesManager::XS_getEntityClass(const std::string &name, CEntity &ret)
{
   CEntity *b = getEntityClass(name);
   if( b )
   {
      ret = *b;
	   
        return true;
   }
   // entity doesn't exist
   return false;
}
 
bool CEntitiesManager::XS_getEntityName(const std::string &name, CEntity &ret)
{
	 
   CEntity *b = getEntityName(name);
   if( b )
   {
 
	   ret = *b;		 			   	   
	   return true;

   }
   // entity doesn't exist
   return false;
}

bool CEntitiesManager::XS_getEntityTargetName(const std::string &name, CEntity &ret)
{
	 
   CEntity *b = getEntityTargetName(name);
   if( b )
   {
 
	   ret = *b;		 			   	   
	   return true;

   }
   // entity doesn't exist
   return false;
}

bool CEntitiesManager::XS_getEntityTarget(const std::string &name, CEntity &ret)
{
	 
   CEntity *b = getEntityTarget(name);
   if( b )
   {
 
	   ret = *b;		 			   	   
	   return true;

   }
   // entity doesn't exist
   return false;
}


void CEntitiesManager::AddRef()
{
        m_refCount++;
}

void CEntitiesManager::Release()
{
        m_refCount--;
}



}