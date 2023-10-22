
#ifndef __XCAMERA_H
#define __XCAMERA_H

namespace X
{
#define XCAMERA_PLATFORM_Z_DISTANCE 150
#define XCAMERA_PLATFORM_Y_DISTANCE 100
#define XCAMERA_ISOMETRIC_Y_DISTANCE 250


enum CameraModes
{
	CAMERA_MODE_ISOMETRIC=0,
	CAMERA_MODE_CUTSCENE,
	CAMERA_MODE_FREE,
	CAMERA_MODE_PLATFORM,
	CAMERA_MODE_STATIC,
	CAMERA_MODE_COUNT
};



// This is our camera class
class CCamera 
{
protected:
	

 
public: 

// The camera's position
	CVector3 m_vPosition;					

	GLfloat fps;

	// The camera's view
	CVector3 m_vView;						

	// The camera's up vector
	CVector3 m_vUpVector;		
	
	// The camera's strafe vector
	CVector3 m_vStrafe;						

	// Our camera constructor
	CCamera();	

	// These are are data access functions for our camera's private data
	CVector3 Position() {	return m_vPosition;		}
	CVector3 View()		{	return m_vView;			}
	CVector3 UpVector() {	return m_vUpVector;		}
	CVector3 Strafe()	{	return m_vStrafe;		}
	 
	void setMode(CameraModes c_Mode);
	int getMode();
	bool isBound();

	//Binds camera to selected actor
	void bindTo(const std::string &ActorName);
	void unbind();
	void setView(const CVector3 &rPosition);
	bool setViewTarget(const CEntity &rTargetEntity);
	bool setViewTarget(const std::string &rUniqueEntityName);
	void setPosition(const CVector3 &rPosition);
	void setUpVector(const CVector3 &rUp);
	

 	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void setPosition(const CVector3 &position, const CVector3 &view, const CVector3 &upVector);
	
	// Set camera properties from a reference to entity
	bool setFromEntity(const CEntity &rCameraEntity);
	
	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(GLfloat angle, GLfloat X, GLfloat Y, GLfloat Z);
	void RotateViewAround(GLfloat x1,GLfloat y1,GLfloat z1, GLfloat angle, GLfloat X, GLfloat Y, GLfloat Z);

	void CalculateFrameRate();
 
	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(CVector3 vCenter, GLfloat X, GLfloat Y, GLfloat Z);

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(GLfloat speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(GLfloat speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

	
  
 	void setFOV(float value, bool fastChange,float speedRate);
	void setFOV(float value);

	float getFOV(){return FOV;}

	CMatrix4x4 getViewMatrix();
	CMatrix4x4 getInverseMatrix();
	CMatrix4x4 getProjectionMatrix();



	  
	void AddRef();
	void Release();
 
private:	
	CMatrix4x4 m_View;
	CMatrix4x4 m_Inverse;
	CMatrix4x4 m_Projection;


	//Cutscene waypoints
	CEntity *pActualWaypoint;
	CEntity *pDestinationWaypoint;
	CEntity *pCameraTarget;
	CVector3 m_vInterpolatedFov;
 

float walkSpeed; 
float runSpeed;
GLfloat deltaTime;
float _FOV;
float FOV;
float _FOVspeedrate;

int cameraMode;

std::string			s_ActorBoundName;



int m_refCount;

	
};


 

}
#endif
