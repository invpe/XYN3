#pragma once	
namespace X
{
	/*!
	* Enumerator to specify if actor is a Player(character controller)
	* or an Object (convexmesh)
	*/
	 enum eActorType
	 {
		 ACTORTYPE_PLAYER=0,
		 ACTORTYPE_OBJECT,
		 ACTORTYPE_UNKNOWN
	 };
	/*!
	* Enumerator to specify the walk direction of the actor
	*/
	 enum eActorWalkDirection
	 {
		 ACTORWALK_FORWARD,
		 ACTORWALK_BACKWARD,
		 ACTORWALK_STOP
	 };
	/*!
	* Actor rotation directions
	*/
	 enum eActorRotationDirection
	 {
		 ACTORROTATE_LEFT,
		 ACTORROTATE_RIGHT,
		 ACTORROTATE_NONE
	  };
	/*!
	* Actor waypoint
	*/
	struct tActorWaypoint
	{
		CEntity *pEntity;
		bool	bLooping;		
	};
	/*!
	* Single actor definition
	*/
	class CActor
	{

	public:

		CActor();
		~CActor();
		/*!
		* Return the model transformation matrix
		*/
		CMatrix4x4 getModelMatrix();
		/*!
		* Return the normal matrix
		*/
		CMatrix3x3 getNormalMatrix();
		/*!
		* Return the Model
		*/
		XModel::XYN *GetModel();
		/*!
		* Get the pointer to CActor entity
		*/
		CEntity* GetEntity();
		/*!
		* Set the entity for this CActor
		*/
		bool SetEntity(CEntity *pEntity);
		/*!
		* Rotate actor to position
		*/
		void RotateTo(const CVector3 &rPosition);
		/*!
		* Rotate actor to entity
		*/
		void RotateTo(const CEntity	 &rEntity);
		/*!
		* Rotate actor to entity
		*/
		void RotateTo(const std::string &rUniqueActorName);
		/*!
		* Rotate an axis to a degree
		*/
		void Rotate(const CVector3 &rAxisDegree);
		/*!
		* Rotate the actor continously by a rotation speed factor
		*/
		void Rotate(int iDirection, float fRotationSpeed);
		/*!
		* Set this actor flag to walk
		*/		
		void Walk(int iDirection,float fWalkSpeed, float fMaxSpeed);	 
		/*!
		* Rotate the actor continously by a rotation speed factor
		*/
		void SetAnimation(const std::string &rAnimationName);
		/*!
		* Clear Angular and Linear velocity
		*/		
		void ClearVelocities();
		/*!
		* Set mass
		*/	
		void SetMass(float mass);
		/*!
		* Set mass
		*/	
		float GetMass();
		/*!
		* Set actor position
		*/
		void SetPosition(const CVector3 &rPosition);
		/*!
		* Activate actor's body (when sleeping)
		*/
		void Activate();
		/*!
		* Full bind 
		*/
		bool Bind(const std::string &rMyBone,const std::string &rToActorName,const std::string &rToActorBoneName);
		/*!
		* Center-of-our-model bind ( no bone position of our model to bind )
		*/
		bool Bind(const std::string &rToActorName,const std::string &rToActorBoneName);
		/*!
		* Add new WPT
		*/
		void AddWaypoint(tActorWaypoint newWaypoint);
		void ClearWaypoints();
		/*!
		* Unbind if anything bound
		*/
		void Unbind();
		/*!
		* Update the player position, rotation and matrices
		*/
		void Tick();
		/*!
		* Returns a rotation vector3 for all axises
		*/
		void GetRotation(CVector3 *vAxis, float *fAngle);
		/*!
		* Return actor position
		*/
		CVector3 GetPosition();
	


		/*!
		* XYN model object
		*/
		XModel::XYN	*m_pModel;
		/*!
		* Model matrix
		*/
	    CMatrix4x4	m_Model;						 
		/*!
		* Normal matrix for this model
		*/
		CMatrix3x3	m_Normal;					 
		/*!
		* Pointer to entity that describes this actor 
		*/
		CEntity		*m_pEntity;
		/*!
		* Actor physic shape
		*/		
		btCollisionShape*	m_pCollisionShape;
		/*!
		* Actor rigid body
		*/
		btRigidBody*		m_pRigidBody;	
		/*!
		* Actor type enumerator
		*/
		enum eActorType		m_eActorType;	
		/*!
		* Current actor rotation angles
		*/
		CVector3			m_vRotation;
		/*!
		* Bind: My bone to bind
		*/
		std::string			m_sBoundBone;
		/*!
		* Bind: To actor 
		*/
		std::string			m_sToActorName;
		/*!
		* Bind: To actor -> Bone
		*/
		std::string			m_sToActorBone;
		/*!
		* Actor waypoints
		*/
		std::vector<tActorWaypoint> m_tWaypoints;
		/*!
		* Current waypoint indicator
		*/
		int							m_iCurrentWaypoint;

		private:
		
	};





	/*!
	* Actors manager class
	*/
	class CActorsManager
	{

	public:

		CActorsManager();
		~CActorsManager();
		/*!
		* Create an actor from map entity
		*/
		bool CreateActor(CEntity *pActorEntity);
		/*!
		* Get a number of actors managed
		*/
		int getActorsCount();
		/*!
		* Return selected actor class
		*/
		CActor *getActor(int actorNumber);
		CActor *getActor(const std::string &rUniqueEntityName);
		int		getActorID(const std::string &rUniqueEntityName);
		/*!
		* Update actors matrices
		*/
		void Tick();
		/*!
		* Render actors with/without a materials
		*/
		void Render(bool bShadowPass);
		/*!
		* Permanently removes the actor from engine
		*/
		void DeleteActor(const std::string &actorName);

		/*!
		* AS Wrappers
		*/
		void XS_Walk(const std::string &actorName,int e_WalkDir,float fSpeed,float fMaxSpeed);
		void XS_Rotate(const std::string &actorName,int e_RotationDir,float fSpeed);
		void XS_RotateTo(const std::string &actorName,const std::string &toActorName);
//		void XS_RotateTo(const std::string &actorName,const std::string &toEntityName);
		void XS_SetAnimation(const std::string &actorName,const std::string &animationName);
		void XS_Bind(const std::string &actorName,const std::string &rMyBone,const std::string &rToActorName,const std::string &rToActorBoneName);
		void XS_Activate(const std::string &actorName);
		void XS_SetPosition(const std::string &actorName, const CVector3 &rPosition);
		void XS_GetEntity(const std::string &actorName, CEntity &rEntity);
		void XS_ClearVelocities(const std::string &actorName);
		void XS_DeleteActor(const std::string &actorName);
		void XS_SetMass(const std::string &actorName, float fMass);
		void XS_GetPosition(const std::string &actorName, CVector3 &rPosition);
		float XS_GetMass(const std::string &actorName);
		void XS_Walk(const std::string &actorName,int e_WalkDir);
		void XS_AddWaypoint(const std::string &actorName, const std::string &uniqueEntityName);
		void XS_ClearWaypoints(const std::string &actorName);
		void AddRef();
		void Release();
	private:

		/*!
		* A vector list containing pointers to actors we manage
		*/
		std::vector<CActor*> m_pActors; 

		/*!
		* AngelScript refCount
		*/
		int m_refCount;
	};




}
