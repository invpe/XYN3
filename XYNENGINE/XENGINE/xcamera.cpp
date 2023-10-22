#include "xmain.h"
  
namespace X
{
 
 

CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		// Init a vector to 0 0 0 for our position
	CVector3 vView = CVector3(0.0, 1.0, 0.5);		// Init a starting view vector (looking up and out the screen) 
	CVector3 vUp   = CVector3(0.0, 1.0, 0.0);		// Init a standard up vector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector

	pActualWaypoint=NULL;
	pDestinationWaypoint=NULL;
}


void CCamera::setMode(CameraModes c_Mode)
{
	cameraMode=c_Mode;
}

int CCamera::getMode()
{
	return cameraMode;
}
void CCamera::setFOV(float value)
{
	_FOV=value;
	FOV=value;
}
void CCamera::setFOV(float value, bool fastChange, float speedRate)
{
	_FOVspeedrate=speedRate;

	if(fastChange==false)_FOV=value;
	else
	{
		_FOV=value;
		FOV=value;
	}
}

bool CCamera::setViewTarget(const CEntity &rTargetEntity)
{
	CEntity *pEntityFound=Game->EntitiesManager()->getEntityName(rTargetEntity.entity_name);
	if(pEntityFound)
	{
		pCameraTarget=pEntityFound;
		return true;
	}

	pCameraTarget=NULL;

}
bool CCamera::setViewTarget(const std::string &rUniqueEntityName)
{
	CEntity *pEntityFound=Game->EntitiesManager()->getEntityName(rUniqueEntityName);
	if(pEntityFound)
	{
		pCameraTarget=pEntityFound;
		return true;
	}

	pCameraTarget=NULL;

}
bool CCamera::setFromEntity(const CEntity &rCameraEntity)
{
	 
	
		
		//	We already know that when a camera is set to be static,
		//	it requires target entity so we can set the view vector.
		//	If this entity is not linked, we return false here.
		if(rCameraEntity.classname=="Camera_static")
		{			
				Game->Log()->write("<%s:%i %s> Camera entering static mode %s ",__FILE__,__LINE__,__FUNCTION__,rCameraEntity.entity_name.c_str());
				CEntity *targetEntity=Game->EntitiesManager()->getEntityTargetName(rCameraEntity.target);
				if(targetEntity!=NULL)
				{
					setMode(CAMERA_MODE_STATIC);
					setFOV(rCameraEntity.fov);					
					setPosition(rCameraEntity.origin);
					setUpVector(CVector3(0,1,0));					
					setView(targetEntity->origin);
					
					return true;
				}
				else
				{
					targetEntity=NULL;
					Game->Log()->write("<%s:%i %s> No target for static camera found. ",__FILE__,__LINE__,__FUNCTION__);
				}

		}
		//	Camera Cutscene entity requires the target entity linked
		//	Also it has a special "LookAt" definition within the entity
		//	That indicates the entity or object we shall be looking at 
		//	at the start
		else if(rCameraEntity.classname=="Camera_cutscene")
		{
			 
				Game->Log()->write("<%s:%i %s> Starting cutscene %s  ",__FILE__,__LINE__,__FUNCTION__,rCameraEntity.entity_name.c_str());

				//	Find the targetName entity
				CEntity *targetEntity=Game->EntitiesManager()->getEntityTargetName(rCameraEntity.target);
				if(targetEntity!=NULL)
				{
					
					//	Set camera mode upvector and fov properly
					setMode(CAMERA_MODE_CUTSCENE);
					setFOV(rCameraEntity.fov);					
					setUpVector(CVector3(0,1,0));


					//	Check if we have a LookAt defined in this entity
					if(rCameraEntity.lookat.empty()==false)
					{
						pCameraTarget=Game->EntitiesManager()->getEntityName(rCameraEntity.lookat);
						if(pCameraTarget==NULL)
						{
							//	Entity name wrong
							Game->Log()->write("<%s:%i %s> Cutscene \"%s\" couldn't resolve the LookAt entity name ",__FILE__,__LINE__,__FUNCTION__,rCameraEntity.entity_name.c_str());
							pCameraTarget=NULL;
							return false;
						}
						else
						{
							Game->Log()->write("<%s:%i %s> Cutscene \"%s\" look at %s ",__FILE__,__LINE__,__FUNCTION__,rCameraEntity.entity_name.c_str(),pCameraTarget->entity_name.c_str());
						}
					}

					//Set the actual waypoint as the current entity
					pActualWaypoint=Game->EntitiesManager()->getEntityClass(rCameraEntity.classname);
					
					//Set the start destination waypoint
					pDestinationWaypoint=targetEntity;

					if(pActualWaypoint!=NULL && pDestinationWaypoint!=NULL)
					return true;
				} 
				else
				{
					targetEntity=NULL;
					Game->Log()->write("<%s:%i %s> No target for cutscene found. ",__FILE__,__LINE__,__FUNCTION__);
				}
		 

		}

	 

	return false;
}


bool CCamera::isBound()
{
	if(s_ActorBoundName.empty()==true)return false;
	else return true;
}


void CCamera::unbind()
{	 
	 s_ActorBoundName.clear();
}


void CCamera::bindTo(const std::string &ActorName)
{
	
	if(ActorName.empty()==false)
	{		
		s_ActorBoundName=ActorName;
	}
 
}
  
 

void CCamera::setView(const CVector3 &rPosition)
{
	m_vView     = rPosition;
}
void CCamera::setPosition(const CVector3 &rPosition)
{
	m_vPosition	= rPosition;
}
void CCamera::setUpVector(const CVector3 &rUp)
{
	m_vUpVector=rUp;
}

void CCamera::setPosition(const CVector3 &position, const CVector3 &view, const CVector3 &upVector)
{
	CVector3 vPosition	= position;
	CVector3 vView		= view;
	CVector3 vUpVector	= upVector;


	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
	
}


CMatrix4x4 CCamera::getViewMatrix()
{
	return m_View;
}

CMatrix4x4 CCamera::getInverseMatrix()
{	
	return m_Inverse;
}
CMatrix4x4 CCamera::getProjectionMatrix()
{
	return m_Projection;
}


void CCamera::SetViewByMouse()
{
	if(  GetKeyState(VK_SHIFT) & 0x80) 
	{
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = GetSystemMetrics(SM_CXSCREEN)  >> 1;				// This is a binary shift to get half the width
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;				// This is a binary shift to get half the height
	GLfloat angleY = 0.0f;							// This is the direction for looking up or down
	GLfloat angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static GLfloat currentRotX = 0.0f;
	
	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);						
	
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;

	// Set the mouse position to the middle of our window
	SetCursorPos(middleX, middleY);							

	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (GLfloat)( (middleX - mousePos.x) ) / 500.0f;		
	angleZ = (GLfloat)( (middleY - mousePos.y) ) / 500.0f;		

	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX -= angleZ;  

	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)
		currentRotX = 1.0f;
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
		currentRotX = -1.0f;
	// Otherwise, we can rotate the view around our position
	else
	{
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		CVector3 vAxis;
		vAxis=vAxis.Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis=vAxis.Normalize(vAxis);

		// Rotate around our perpendicular axis and along the y-axis
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
		RotateView(angleY, 0, 1, 0);
	}


	}//keystate
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	CVector3 vNewView;

	// Get the view vector (The direction we are facing)
	CVector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	GLfloat cosTheta = (GLfloat)cos(angle);
	GLfloat sinTheta = (GLfloat)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}





/*
*  x1,y1,z1 - tam gdzie kamera ma patrzec
*  angle - rotation value
*  x- odleglosc od x1
*  y- wysokosc
*  z- odleglosc z
*/
void CCamera::RotateViewAround(GLfloat x1,GLfloat y1,GLfloat z1, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)

{
CVector3 vNewView;
CVector3 vNewPosition; 
CVector3 vCenter;

vCenter.x=x1;
vCenter.y=y1;
vCenter.z=z1;
//glLoadIdentity();
vNewPosition = vCenter;

vNewPosition.x -= (cosf(-angle * (3.14f/180.0f)) * x);
if(y>0)vNewPosition.y += y;
vNewPosition.z -= (sinf(-angle * (3.14f/180.0f)) * z);

m_vView = vCenter;
m_vPosition = vNewPosition;

}

void CCamera::StrafeCamera(GLfloat speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

void CCamera::MoveCamera(GLfloat speed)
{
	// Get the current view vector (the direction we are looking)
	CVector3 vVector = m_vView - m_vPosition;
	vVector = vVector.Normalize(vVector);

	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.y += vVector.y * speed;		// Add our acceleration to our position's Y
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.y += vVector.y * speed;			// Add our acceleration to our view's Y
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}

void CCamera::CheckForMovement()
{	
	// Once we have the frame interval, we find the current speed
 	//GLfloat speed = kSpeed * g_FrameInterval;
	GLfloat speed=0.05*Game->getDelta();

	 
	if(  GetKeyState(VK_SHIFT) & 0x80) 
	{		 
		   
							if(Game->getKeyState(X::XKeys::KEYS)==X::XKeyState::DOWN)	
							{
								MoveCamera(-speed);			
							}
							if(Game->getKeyState(X::XKeys::KEYW)==X::XKeyState::DOWN)	
							{
								MoveCamera(speed);			
							}
							if(Game->getKeyState(X::XKeys::KEYA)==X::XKeyState::DOWN)	
							{
								StrafeCamera(-speed);
							}
							if(Game->getKeyState(X::XKeys::KEYD)==X::XKeyState::DOWN)	
							{
								StrafeCamera(speed);	
							}
				 

	}


}


void CCamera::Update() 
{

	//FOV Updates
	if(FOV<_FOV)
	{
		FOV+=_FOVspeedrate+Game->getDelta()/1000;
	}
	if(FOV>_FOV)
	{
		FOV-=_FOVspeedrate+Game->getDelta()/1000;
	}

			switch(getMode())
			{

 

			case CAMERA_MODE_FREE:
			{
				CVector3 vCross = vCross.Cross(m_vView - m_vPosition, m_vUpVector);
				m_vStrafe = m_vStrafe.Normalize(vCross);
				SetViewByMouse();
				CheckForMovement();

			}
			break;

			case CAMERA_MODE_STATIC:
			{
					if(pCameraTarget!=NULL)
					{						
						setView(pCameraTarget->origin);
					}
				 		 
			}
			break;


			//Interpolated camera movement
			case CAMERA_MODE_CUTSCENE:
					{
						 					
						 
						if(pActualWaypoint!=NULL && pDestinationWaypoint!=NULL)
						{
								static float mu=0.0f;
							

								// Interpolate time to create smooth speed changes
								float time = pActualWaypoint->time * (1.0f - mu) + pDestinationWaypoint->time * mu;

								if ( time > 0 )
									mu = mu + ( Game->getDelta()/1000.0f  ) / time;
								else
									mu = 1.0f;
 
								//	We have reached the destination
								if ( mu >= 1.0f )
								{
									
									//	Set actual WPT as the last DST WPT
									pActualWaypoint=pDestinationWaypoint;	

									//	Get the new destination WPT
									pDestinationWaypoint=Game->EntitiesManager()->getEntityTargetName(pActualWaypoint->target);
 
							 		//	End of cutscene, no target for actual WPT.
									if(pActualWaypoint->target.empty()==true)
									{

										Game->Log()->write("<%s:%i %s> Cutscene finished",__FILE__,__LINE__,__FUNCTION__);
										break;

										 
										
									} 





									//	Check if we have a LookAt defined in this entity
									if(pActualWaypoint->lookat.empty()==false)
									{
										pCameraTarget=Game->EntitiesManager()->getEntityName(pDestinationWaypoint->lookat);
										if(pCameraTarget==NULL)
										{
											//	Entity name wrong
											Game->Log()->write("<%s:%i %s> Cutscene   couldn't resolve the LookAt \"%s\" entity name ",__FILE__,__LINE__,__FUNCTION__,pActualWaypoint->lookat.c_str());
											pCameraTarget=NULL;		
											setView(CVector3(0,0,0));
										}
										else
										{
											Game->Log()->write("<%s:%i %s> Cutscene   look at %s ",__FILE__,__LINE__,__FUNCTION__, pCameraTarget->entity_name.c_str());
										}
									}

									mu = 0;
								}

								//	Perform Linear interpolation
								m_vPosition = Interpolate::Linear( pActualWaypoint->origin, pDestinationWaypoint->origin,mu );	
								m_vInterpolatedFov = Interpolate::Linear(CVector3(pActualWaypoint->fov,0,0),CVector3(pDestinationWaypoint->fov,0,0),mu);
								//	Perform Linear view vector interpolation if target existing
								if(pCameraTarget!=NULL)
								{
									m_vView = Interpolate::Linear(m_vView , pCameraTarget->origin,mu );	
								}
								//	Set interpolated FOV
								setFOV(m_vInterpolatedFov.x);
 
						}
					
					}
				break;
			}
	 

	 
	//Update the camera projection matrix
	m_Projection.createProjectionMatrix(getFOV(),(int)Game->getScreenSize().x/(int)Game->getScreenSize().y,1,1500);

	//Update the camera view matrix
	m_View.createViewMatrix(m_vPosition,m_vView,m_vUpVector);
	
	//Update the camera inverse matrix
	m_Inverse.invertMatrix(m_View);
	 
	
}
  

void CCamera::AddRef()
{
        m_refCount++;
}

void CCamera::Release()
{
        m_refCount--;
}


}