#ifndef _XPHYSICS_H
#define _XPHYSICS_H


namespace X
{


	//These are the collision groups
	enum XCollisionGroups
   {

		   //We start from 64, as bullet has something defined before.
		   GHOSTS = 64,
		   WALLS = 128,
		   ACTORS = 256,
		   PLAYERS = btBroadphaseProxy::CharacterFilter
		 
		
   }; 
	

	//These are the masks for collision detection
	enum XCollidesWith
	{
		WITH_EVERYTHING = XCollisionGroups::ACTORS | XCollisionGroups::WALLS | XCollisionGroups::PLAYERS,
		WITH_GHOSTS		=  XCollisionGroups::GHOSTS,
		WITH_WALLS		=  XCollisionGroups::WALLS,
		WITH_ACTORS		=  XCollisionGroups::ACTORS,
	    WITH_NOTHING = 0
	};




	class CPhysicsManager
	{

	public:
		CPhysicsManager();
		~CPhysicsManager();
		btBroadphaseInterface*	m_broadphase;
		void Init(const CVector3 &p_Gravity,const CVector3 &p_WorldMax,const  CVector3 &p_WorldMin);
		void Tick();
		bool isInitialized();
		void addConvexVerticesCollider(btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation);
		btRigidBody* localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
		btDiscreteDynamicsWorld*  getDynamic();
		void helperRagDoll (const btVector3& positionOffset,btScalar scale_ragdoll);
		void setPhysicsDebugDrawing(bool state);
		bool getPhysicsDebugDrawing();
		void AddRef();
		void Release();
	private:
	 	bool initialized;

		btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
		btCollisionDispatcher*	m_dispatcher;
		btConstraintSolver*	m_solver;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btDiscreteDynamicsWorld* m_dynamicsWorld;

		 
		 GLDebugDrawer sDebugDraw;
			bool physicsDrawDebug;


int m_refCount;
	};

}
#endif