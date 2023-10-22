#include "xmain.h"

namespace X
{


	CActor::CActor()
	{
		m_eActorType		=	ACTORTYPE_UNKNOWN;
		m_pEntity			=	NULL;
		m_pModel			=	new XModel::XYN;
		m_pCollisionShape	=	NULL;
		m_pRigidBody		=	NULL;
		m_vRotation			=	CVector3(0,0,0);
		m_iCurrentWaypoint	=	0;
		m_tWaypoints.clear();
		m_Model.Clear();
		m_Normal.identity();

	}

	CActor::~CActor()
	{
		
		if(m_pModel)
		{
			Game->Log()->write("<%s:%i %s> Destroying model. ",__FILE__,__LINE__,__FUNCTION__);			
			delete m_pModel;
			m_pModel=NULL;
			Game->Log()->write("<%s:%i %s> Destroying model OK ",__FILE__,__LINE__,__FUNCTION__);
		}
		if (m_pCollisionShape)
		{
			Game->Log()->write("<%s:%i %s> Destroying m_pCollisionShape. ",__FILE__,__LINE__,__FUNCTION__);
			delete m_pCollisionShape;
			m_pCollisionShape=NULL;
			Game->Log()->write("<%s:%i %s> Destroying m_pCollisionShape OK. ",__FILE__,__LINE__,__FUNCTION__);
		}
		if (m_pRigidBody)
		{
			Game->Log()->write("<%s:%i %s> Destroying m_pRigidBody. ",__FILE__,__LINE__,__FUNCTION__);
			Game->Physics()->getDynamic()->removeRigidBody(m_pRigidBody);				
			delete m_pRigidBody;
			m_pRigidBody = NULL;
			Game->Log()->write("<%s:%i %s> Destroying m_pRigidBody OK. ",__FILE__,__LINE__,__FUNCTION__);
		}

	}

	bool CActor::SetEntity(CEntity *pEntity)
	{
		if(pEntity)
		{
			m_pEntity=pEntity;						
			return true;
		}

		return false;
	}

	XModel::XYN *CActor::GetModel()
	{
		if(m_pModel)return m_pModel;
		else return NULL;
	}

	CEntity* CActor::GetEntity()
	{
		if(m_pEntity)return m_pEntity;
		else return NULL;
	}
	CVector3 CActor::GetPosition()
	{
	 return CVector3(m_Model.GetColumn(3).x,m_Model.GetColumn(3).y,m_Model.GetColumn(3).z);
	}
	void CActor::RotateTo(const CEntity	 &rEntity)
	{
		float rv=mathGetRotationToVector(GetPosition(),rEntity.origin)+180;
		Rotate(CVector3(0,rv,0));		

	}
	void CActor::RotateTo(const CVector3 &rPosition)
	{
		float rv=mathGetRotationToVector(GetPosition(),rPosition)+180;
		Rotate(CVector3(0,rv,0));		
	}
	void CActor::RotateTo(const std::string &rUniqueActorName)
	{
		CActor *pActorToRotateTo=Game->ActorsManager()->getActor(rUniqueActorName);
		if(pActorToRotateTo)
		{	
			RotateTo(pActorToRotateTo->GetPosition());
		}
	}
	void CActor::Rotate(const CVector3 &rAxisDegree)
	{
	 
		btTransform xform;
		CMatrix4x4 m_Current;
		m_pRigidBody->getWorldTransform().getOpenGLMatrix(m_Current);

		CMatrix4x4 m_Rotation;		 	
		CQuaternion a;			  
		a.RotationRadiansf(GET_RADIANS(rAxisDegree.x),GET_RADIANS(rAxisDegree.y),GET_RADIANS(rAxisDegree.z));
		a.CreateMatrix(m_Rotation);
		m_Current*=m_Rotation;	 

		xform.setFromOpenGLMatrix(m_Current);

		m_pRigidBody->setWorldTransform(xform);
	
		m_vRotation.x=GET_RADIANS(rAxisDegree.x);
		m_vRotation.y=GET_RADIANS(rAxisDegree.y);
		m_vRotation.z=GET_RADIANS(rAxisDegree.z);
		
	}
	void CActor::Rotate(int iDirection, float fRotationSpeed)
	{
	
		btTransform xform;
		m_pRigidBody->getMotionState()->getWorldTransform (xform);

		if(iDirection == ACTORROTATE_RIGHT)
		{
			m_vRotation.y  -=  fRotationSpeed + Game->getDelta() / 1000.0f;			
			
		}
		else if(iDirection == ACTORROTATE_LEFT)
		{
			m_vRotation.y  +=  fRotationSpeed + Game->getDelta() / 1000.0f;
			
		}
		xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_vRotation.y));
		m_pRigidBody->getMotionState()->setWorldTransform (xform);
		m_pRigidBody->setCenterOfMassTransform (xform);
			
	}
	void CActor::GetRotation(CVector3 *vAxis, float *fAngle)
	{
		CVector3 vReturn;
		btTransform xform;
		xform=m_pRigidBody->getWorldTransform();
		btQuaternion quat;
		quat=xform.getRotation();
		 
		float scale = sqrt(quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z());
		vAxis->x = quat.x() / scale;
		vAxis->y  = quat.y() / scale;
		vAxis->z  = quat.z() / scale;
		*fAngle = GET_DEGREES(acos(quat.w()) * 2.0f);
	} 
	 
	void CActor::Walk(int iDirection,float fWalkSpeed, float fMaxSpeed)
	{
  	
		btTransform xform;
		m_pRigidBody->getMotionState()->getWorldTransform (xform);

		btVector3 linearVelocity = m_pRigidBody->getLinearVelocity();
		btScalar speed = m_pRigidBody->getLinearVelocity().length();


		btVector3 forwardDir=btVector3(sin(m_vRotation.y),0,cos(m_vRotation.y));		
		btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
		btScalar walkSpeed = fWalkSpeed + Game->getDelta()/1000;

 		if(iDirection == ACTORWALK_FORWARD)
			walkDirection -= forwardDir;					
		 else if(iDirection == ACTORWALK_BACKWARD)
			walkDirection += forwardDir;
				 
		//	Limit the speed
		if (speed < fMaxSpeed)
		{
			btVector3 velocity = linearVelocity + walkDirection * walkSpeed;
			m_pRigidBody->setLinearVelocity (velocity);
		}
		m_pRigidBody->getMotionState()->setWorldTransform (xform);
		m_pRigidBody->setCenterOfMassTransform (xform);

	}
	void CActor::ClearVelocities()
	{
		m_pRigidBody->setAngularVelocity(btVector3(0,0,0));	
		m_pRigidBody->setLinearVelocity(btVector3(0,0,0));	
	}
	void CActor::Activate()
	{
		m_pRigidBody->activate(true);
	}
	void CActor::SetPosition(const CVector3 &rPosition)
	{
		btTransform xform;		
		xform.setIdentity();
		xform.setOrigin(btVector3(rPosition.x,rPosition.y,rPosition.z));
		m_pRigidBody->getMotionState()->setWorldTransform (xform);
		m_pRigidBody->setCenterOfMassTransform (xform);
	}
	bool CActor::Bind(const std::string &rMyBone,const std::string &rToActorName,const std::string &rToActorBoneName)
	{
		//Check if a bone with that name exists
		if(m_pModel->getBone(rMyBone.c_str())!=NULL)
		{
			//Check if an actor with that name exists
			if(Game->ActorsManager()->getActor(rToActorName)!=NULL)
			{
				//Check if that actor has a bone with than name
				if(Game->ActorsManager()->getActor(rToActorName)->m_pModel->getBone(rToActorBoneName.c_str())!=NULL)
				{
					// Destroy our rigid body
					Game->Physics()->getDynamic()->removeRigidBody(m_pRigidBody);		

					m_sBoundBone=rMyBone;
					m_sToActorName=rToActorName;
					m_sToActorBone=rToActorBoneName;
					return true;
				}
			}
		}
		return false;
	}
	void CActor::SetMass(float mass)
	{
		 	btScalar bmass(mass);
			btVector3 inertia(0,0,0);
			m_pCollisionShape->calculateLocalInertia(bmass, inertia);			
			m_pRigidBody->setMassProps(btScalar(bmass),inertia);
			
	}
	float CActor::GetMass()
	{
		if(m_pRigidBody->getInvMass()>0)
		return 1./m_pRigidBody->getInvMass();
		else return m_pRigidBody->getInvMass();
	}
	bool CActor::Bind(const std::string &rToActorName,const std::string &rToActorBoneName)
	{
	
			//Check if an actor with that name exists
			if(Game->ActorsManager()->getActor(rToActorName)!=NULL)
			{
				//Check if that actor has a bone with than name
				if(Game->ActorsManager()->getActor(rToActorName)->m_pModel->getBone(rToActorBoneName.c_str())!=NULL)
				{
					// Destroy our rigid body
					Game->Physics()->getDynamic()->removeRigidBody(m_pRigidBody);	

					m_sBoundBone.empty();
					m_sToActorName=rToActorName;
					m_sToActorBone=rToActorBoneName;
					return true;
				}
			}
		
		return false;
	}
	void CActor::Unbind()
	{
		if(m_sBoundBone.empty()==false || m_sToActorName.empty()==false || m_sToActorBone.empty()==false)
		{
			m_sBoundBone.clear();
			m_sToActorName.clear();
			m_sToActorBone.clear();

			if(m_eActorType=ACTORTYPE_OBJECT)	Game->Physics()->getDynamic()->addRigidBody(m_pRigidBody,X::XCollisionGroups::ACTORS,X::XCollidesWith::WITH_EVERYTHING);	
			if(m_eActorType=ACTORTYPE_PLAYER)	Game->Physics()->getDynamic()->addRigidBody(m_pRigidBody,X::XCollisionGroups::PLAYERS,X::XCollidesWith::WITH_EVERYTHING);	
		}

	}
	void CActor::ClearWaypoints()
	{
		m_tWaypoints.clear();
		m_iCurrentWaypoint=0;
	}
	void CActor::AddWaypoint(tActorWaypoint newWaypoint)
	{
		if(newWaypoint.pEntity!=NULL)
		{
			m_tWaypoints.push_back(newWaypoint);
		}
	}
	void CActor::SetAnimation(const std::string &rAnimationName)
	{
		if(m_pModel->getModel()->animated==true)
		{
			m_pModel->setAnimation(rAnimationName);
		}
	}
	void CActor::Tick()
	{
			 
			//	Update animation on objects that are animated
			//  Only the objectss within model that have animation will be updated
			GetModel()->UpdateModel();
			
			
			//	Actor bound
			if(m_sToActorName.empty()==false && m_sToActorBone.empty()==false)
			{

				// Get the required pointers
				CActor *pActorBound=Game->ActorsManager()->getActor(m_sToActorName);				
				XModel::tBoneData *pHisBone=pActorBound->m_pModel->getBone(m_sToActorBone.c_str());

				// We bind by providing our bone position (Full bind)
				if(m_sBoundBone.empty()==false)
				{
					XModel::tBoneData *pMyBone=m_pModel->getBone(m_sBoundBone.c_str());

					if(pActorBound && pMyBone && pHisBone)
					{		
						
						// This is important, we negate Y value here ( strange! )
						CMatrix4x4 mMyBoneMatrix;
						mMyBoneMatrix.Translate(pMyBone->v_ActualPosition.x,-pMyBone->v_ActualPosition.y,pMyBone->v_ActualPosition.z);

						btTransform t_Trans;
						t_Trans.setFromOpenGLMatrix(  ( pActorBound->m_Model *  (  pHisBone->getPositionMatrix() * pHisBone->getRotationMatrix() * mMyBoneMatrix ) ) );	
						
						m_pRigidBody->setWorldTransform(t_Trans);	
						m_pRigidBody->setCenterOfMassTransform(t_Trans);
						
					}
				}
				// Bind without providing our bone position center-of-the-model bind ( we will bind to the center of the model, instead of our bone )
				else
				{
							btTransform t_Trans;
							t_Trans.setFromOpenGLMatrix(  ( pActorBound->m_Model *  (  pHisBone->getPositionMatrix() * pHisBone->getRotationMatrix() ) ) );							
							m_pRigidBody->setWorldTransform(t_Trans);
							m_pRigidBody->setCenterOfMassTransform(t_Trans);
				}



			}
			// Actor not bound
			else
			{


						// Check if actor is waypointed, if so - we shall overwrite other movements
						if(m_tWaypoints.size()>0)
						{
								// Rotate to the destination
								RotateTo(m_tWaypoints[m_iCurrentWaypoint].pEntity->origin);
			 
								//
								Walk(ACTORWALK_FORWARD,0.5,0.5);

								// Calculate the minima distance and go to next wpt if possible
								CVector3 t;
								float d=t.Distance(GetPosition(),m_tWaypoints[m_iCurrentWaypoint].pEntity->origin);
								if(d<35)
								{
									if(m_tWaypoints.size()>m_iCurrentWaypoint+1)
									{
										m_iCurrentWaypoint++;
									}
									else
									{
										if(m_tWaypoints[m_iCurrentWaypoint].bLooping==true)
										{
											m_iCurrentWaypoint=0;
										}
										else
										{
											m_tWaypoints.clear();
											m_iCurrentWaypoint=0;
										}
									}
								}
						}
			 
			 			/*

								//DOORS specific Physique handling
								if(strstr(Actors[it]->sModelName.c_str(),"doors"))								
								{
									btTransform t_Trans;
									t_Trans.setFromOpenGLMatrix(Actors[it]->m_getModel()*Actors[it]->pModel->getBone("BoneHINGE")->getTranslationMatrix()*Actors[it]->pModel->getBone("BoneHINGE")->getRotationMatrix());
									Actors[it]->p_Body->setWorldTransform(t_Trans);									
								}
								else
								{
									Actors[it]->p_Body->getWorldTransform().getOpenGLMatrix(Actors[it]->m_Model);
								}
							 	 
						*/

			}
		 
			//	Physic to GL matrix
			m_pRigidBody->getWorldTransform().getOpenGLMatrix(m_Model);					 

			//	Update the entity position 
			GetEntity()->origin=GetPosition();
			

	}




	void CActorsManager::AddRef()
	{
			m_refCount++;
	}

	void CActorsManager::Release()
	{
			m_refCount--;
	}
	CActorsManager::CActorsManager()
	{
	}

	CActorsManager::~CActorsManager()
	{
		for(int a=0;a<m_pActors.size();a++)
		{
			m_pActors[a]->~CActor();
		}
	}

	int CActorsManager::getActorsCount()
	{
		return m_pActors.size();
	}
	void CActorsManager::Render(bool bShadowPass)
	{
	X::Shader *pActiveShader;

	if(m_pActors.size()>0)
	{
		Game->StateManager()->Set(CULLSIDE, CULLSIDE_BACK);				
		int it=0;	
		int obj=0;
		int matCt=0;
		for(it=0;it<m_pActors.size();it++)
		{

			
				if(bShadowPass==true)
				{
					m_pActors[it]->m_pModel->renderModel(true);
				}
				else
				{

					//We are to bind the materials here, so we need to iterate throguh obbjects in the model
					//bind appropriate materials and set matrices					
					for(obj=0;obj<m_pActors[it]->m_pModel->getModel()->numOfObjects;obj++)
					{						
						for(matCt=0;matCt<m_pActors[it]->m_pModel->getModel()->pObject[obj].materialID.size();matCt++)
						{
							X::tMaterial *material=Game->MaterialsManager()->getMaterial(m_pActors[it]->m_pModel->getModel()->pMaterials[m_pActors[it]->m_pModel->getModel()->pObject[obj].materialID[matCt]]->strName);
							if(material!=NULL)
							{	
							  
											//This whole process is done once per material
											pActiveShader=NULL;
											pActiveShader=Game->ShadersManager()->getShader(material->sMaterialShader);
											if(pActiveShader!=NULL)
											{
													//Turn on and prepare per-shader data
													pActiveShader->TurnOn();
				
													//Pass standard matrices
													pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_Projection"),		Game->Camera()->getProjectionMatrix()	);
													pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_View"),				Game->Camera()->getViewMatrix()	); 		
													pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_Model"),			m_pActors[it]->m_Model		); 
														

													//When lighting required, pass the per-light data
													if(material->bCalculateLighting==true)
													{
														Game->LightsManager()->passLightsToShader(pActiveShader);
			
														m_pActors[it]->m_Normal.createNormalMatrix(Game->Camera()->getViewMatrix()*m_pActors[it]->m_Model);
														pActiveShader->SetMatrix3x3(pActiveShader->getUniform("m_Normal"),m_pActors[it]->m_Normal	); 

														//Pass the standard material structure
														pActiveShader->SetFloat4(pActiveShader->getUniform("Material.ambient"),material->vAmbient.x,material->vAmbient.y,material->vAmbient.z,material->vAmbient.w);
														pActiveShader->SetFloat4(pActiveShader->getUniform("Material.diffuse"),material->vDiffuse.x,material->vDiffuse.y,material->vDiffuse.z,material->vDiffuse.w);
														pActiveShader->SetFloat4(pActiveShader->getUniform("Material.specular"),material->vSpecular.x,material->vSpecular.y,material->vSpecular.z,material->vSpecular.w);
														pActiveShader->SetFloat(pActiveShader->getUniform("Material.shininess"),material->fShininess);

													}

													//When material is to be shadowed, pass cameraInverse matrix
													if(material->bCalculateShadows==true)
													{
														pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_cameraInverse"),	Game->Camera()->getInverseMatrix()		);	
													}
				
													//Bind textures associated with this material
													int iter;				
													for(iter=0;iter<material->Textures.size();iter++)
													{
														//Setup the bind number for the texture
														glActiveTexture(GL_TEXTURE0+material->Textures[iter]->i_ActiveTextureBindNumber);

														//Get and bind the texture
														X::XTexture *faceTexture=Game->TexturesManager()->getTexture(material->Textures[iter]->s_TextureName);
														if(faceTexture!=NULL)glBindTexture(GL_TEXTURE_2D, faceTexture->u_Texture);
													}
 
													m_pActors[it]->m_pModel->renderObject(obj,false);		
													pActiveShader->TurnOff();
													pActiveShader=NULL;

											}//activeShader
							}//material!=NULL					
						}//materials count
					}//objects in model count
				}//shadowpass = false
		}//for actors size
		Game->StateManager()->Set(CULLSIDE, CULLSIDE_FRONT);
	}//if actors size >0

	}


	void CActorsManager::Tick()
	{
		 int it=0;
		 for(it=0;it<m_pActors.size();it++)
		 {
			 //	Update the actor internal data
			 m_pActors[it]->Tick();
	
		 }	
	}
	bool CActorsManager::CreateActor(CEntity *pActorEntity)
	{
		Game->Log()->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);

		if(pActorEntity)
		{
			//	Create new Actor object
			CActor *pNewActor=new CActor;

			//	Set the actor entity
			pNewActor->m_pEntity=pActorEntity;

			//	Load XYN model for new actor
			if(pNewActor->GetEntity()->model_name.empty()==false)
			{
				//	Load model
				if(pNewActor->m_pModel->LoadModel("data\\models\\"+pNewActor->GetEntity()->model_name,pNewActor->GetEntity()->model_name+".xyn")==false)
				{
					//	Problem loading model
					return false;
				}

				//	If animated, set to idle as default
				if(pNewActor->m_pModel->getModel()->animated==true)
				{
					pNewActor->m_pModel->setAnimation("IDLE");
				}
 
				//	Create physics object for this actor
				if(pNewActor->m_pEntity->bCharacter==true)
				{
					pNewActor->m_eActorType=ACTORTYPE_PLAYER;
	 							
					btScalar characterHeight=pNewActor->m_pModel->getModel()->modelSize.y;
					btScalar characterWidth =pNewActor->m_pModel->getModel()->modelSize.x;
					pNewActor->m_pCollisionShape		=	new btCapsuleShape(characterWidth,characterHeight);

					btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(pNewActor->GetEntity()->origin.x,pNewActor->GetEntity()->origin.y,pNewActor->GetEntity()->origin.z)));
					btScalar mass(pNewActor->GetEntity()->weight);
					btVector3 inertia(0,0,0);
					pNewActor->m_pCollisionShape->calculateLocalInertia(mass, inertia);
					btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,myMotionState,pNewActor->m_pCollisionShape,inertia);
					
					pNewActor->m_pRigidBody=new btRigidBody(fallRigidBodyCI);	
					pNewActor->m_pRigidBody->setCollisionFlags(pNewActor->m_pRigidBody->getCollisionFlags() );		
					pNewActor->m_pRigidBody->setSleepingThresholds(0.0, 0.0);
					pNewActor->m_pRigidBody->setAngularFactor(0.0);
					

					
					Game->Physics()->getDynamic()->addRigidBody(pNewActor->m_pRigidBody,X::XCollisionGroups::PLAYERS,X::XCollidesWith::WITH_EVERYTHING);								
						

				}
				else
				{
					pNewActor->m_eActorType=ACTORTYPE_OBJECT;


					//	Convert mesh to physics 
					btAlignedObjectArray<btVector3>	vertices;
					for(int a=0;a<pNewActor->m_pModel->getModel()->numOfObjects;a++)
					{
					 	XModel::x3DObject *pObject = &(pNewActor->m_pModel->getModel()->pObject[a]);
						for(int i=0;i<pObject->numOfFaces;i++)
						{
							for(int whichVertex = 0; whichVertex < 3; whichVertex++)
							{
								vertices.push_back(btVector3(pObject->pVertexy[pObject->pFaces[i].vertIndex[whichVertex]].x,pObject->pVertexy[pObject->pFaces[i].vertIndex[whichVertex]].y,pObject->pVertexy[pObject->pFaces[i].vertIndex[whichVertex]].z));
							}
						}
					}

					pNewActor->m_pCollisionShape		=	new btConvexHullShape(&(vertices[0].getX()),vertices.size());
					pNewActor->m_pCollisionShape->setMargin(btScalar(1.1));

					
					btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(pNewActor->GetEntity()->origin.x,pNewActor->GetEntity()->origin.y,pNewActor->GetEntity()->origin.z)));
					btScalar mass( pNewActor->GetEntity()->weight );
					btVector3 inertia(0,0,0);
					pNewActor->m_pCollisionShape->calculateLocalInertia(mass, inertia);
					btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,myMotionState,pNewActor->m_pCollisionShape,inertia);
					
					pNewActor->m_pRigidBody	=	new btRigidBody(fallRigidBodyCI);	
					Game->Physics()->getDynamic()->addRigidBody(pNewActor->m_pRigidBody,X::XCollisionGroups::ACTORS,X::XCollidesWith::WITH_EVERYTHING);								

				}

			}else return false;


			//	Finally, add this actor to our actors list
			m_pActors.push_back(pNewActor);


			return true;
		}

		Game->Log()->write("<%s:%i %s> Warrning, couldn't create new actor",__FILE__,__LINE__,__FUNCTION__);
		return false;
	}
	int	CActorsManager::getActorID(const std::string &rUniqueEntityName)
	{
		int it;
		for(it=0;it<m_pActors.size();it++)
		{
			if(m_pActors[it]->GetEntity()->entity_name==rUniqueEntityName)return it;
		}

	}
	void CActorsManager::DeleteActor(const std::string &actorName)
	{
		CActor *pActorDelete=getActor(actorName);
		if(pActorDelete)
		{
			Game->Log()->write("<%s:%i %s> Deleting %s",__FILE__,__LINE__,__FUNCTION__,actorName.c_str());
			// Check if actor is bound to 

			// Check if other actor is bound to this actor

			// Check if light is bound to this actor

			// Delete the pointer
			pActorDelete->~CActor();
			// Remove from the list so we won't process it anymore
			m_pActors.erase(m_pActors.begin()+getActorID(actorName));


			
		}
	}
	CActor *CActorsManager::getActor(int actorNumber)
	{
		if(actorNumber<m_pActors.size())return m_pActors[actorNumber];
		else return NULL;
	}

	CActor *CActorsManager::getActor(const std::string &rUniqueEntityName)
	{
		int it;
		for(it=0;it<m_pActors.size();it++)
		{
			if(m_pActors[it]->GetEntity()->entity_name==rUniqueEntityName)return m_pActors[it];
		}
		return NULL;
	}
	void CActorsManager::XS_Walk(const std::string &actorName,int e_WalkDir,float fSpeed,float fMaxSpeed)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->Walk(e_WalkDir,fSpeed,fMaxSpeed);
		 }
	}
	void CActorsManager::XS_Rotate(const std::string &actorName,int e_RotationDir,float fSpeed)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->Rotate(e_RotationDir,fSpeed);
		 }
	}
	void CActorsManager::XS_SetAnimation(const std::string &actorName,const std::string &animationName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->SetAnimation(animationName);
		 }

	}
	void CActorsManager::XS_RotateTo(const std::string &actorName,const std::string &toActorName)
	{
		 CActor *pActor=getActor(actorName);	 
		 if(pActor!=NULL)
		 {
			  
				pActor->RotateTo(toActorName);
			 
		 }

	}
	/*
	void CActorsManager::XS_RotateTo(const std::string &actorName,const std::string &toEntityName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 CEntity *pEntity=Game->EntitiesManager()->getEntityName(toEntityName);
			 if(pEntity!=NULL)
			 {
				 pActor->RotateTo(pEntity);
			 }
			 
		 }

	}*/
	void CActorsManager::XS_Bind(const std::string &actorName,const std::string &rMyBone,const std::string &rToActorName,const std::string &rToActorBoneName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->Bind(rMyBone,rToActorName,rToActorBoneName);
		 }

	}
	void CActorsManager::XS_Activate(const std::string &actorName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->Activate();
		 }
	}
	void CActorsManager::XS_SetPosition(const std::string &actorName, const CVector3 &rPosition)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 pActor->SetPosition(rPosition);
		 }
	}
	void CActorsManager::XS_GetEntity(const std::string &actorName, CEntity &rEntity)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			 rEntity=*pActor->m_pEntity;
		 }
	}
	void CActorsManager::XS_ClearVelocities(const std::string &actorName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			pActor->ClearVelocities();
		 }
	}
	void CActorsManager::XS_DeleteActor(const std::string &actorName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			DeleteActor(actorName);
		 }

	}
	void CActorsManager::XS_SetMass(const std::string &actorName, float fMass)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			pActor->SetMass(fMass);
		 }
	}
	float CActorsManager::XS_GetMass(const std::string &actorName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			return pActor->GetMass();
		 }
	}
	void CActorsManager::XS_GetPosition(const std::string &actorName, CVector3 &rPosition)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			rPosition=pActor->GetPosition();
		 }
	}
	void CActorsManager::XS_AddWaypoint(const std::string &actorName, const std::string &uniqueEntityName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			CEntity *pEntity=Game->EntitiesManager()->getEntityName(uniqueEntityName);
			if(pEntity!=NULL)
			{
				X::tActorWaypoint tWpt;
				tWpt.pEntity=pEntity;
				pActor->AddWaypoint(tWpt);
			}
		 }
	} 
	void CActorsManager::XS_ClearWaypoints(const std::string &actorName)
	{
		 CActor *pActor=getActor(actorName);
		 if(pActor!=NULL)
		 {
			pActor->ClearWaypoints();
		 }
	}
}