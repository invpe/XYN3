
#include "xmain.h" 

 
namespace X
{

	
void CPhysicsManager::AddRef()
{
        m_refCount++;
}

void CPhysicsManager::Release()
{
        m_refCount--;
}


	CPhysicsManager::CPhysicsManager()
	{
 
		initialized=false;
		physicsDrawDebug=false;
	}

	CPhysicsManager::~CPhysicsManager()
	{
 
	Game->Log()->write("<%s:%i %s> ",__FILE__,__LINE__,__FUNCTION__);	
//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;
	initialized=false;
	}






	void CPhysicsManager::Init(const CVector3 &p_Gravity,const CVector3 &p_WorldMax,const  CVector3 &p_WorldMin)
	{ 
		initialized=false;
		 
		Game->Log()->write("<%s:%i %s> Initializing Physics subsystem.",__FILE__,__LINE__,__FUNCTION__);

		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

		btVector3 worldMin(p_WorldMin.x,p_WorldMin.y,p_WorldMin.z);
		btVector3 worldMax(p_WorldMax.x,p_WorldMax.y,p_WorldMax.z);
		btAxisSweep3* sweepBP = new btAxisSweep3(worldMin,worldMax);
	 
		m_broadphase=sweepBP;
		m_broadphase = new btDbvtBroadphase();
	 	m_solver = new btSequentialImpulseConstraintSolver();
	 	
		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
		m_dynamicsWorld->setGravity(btVector3(p_Gravity.x,p_Gravity.y,p_Gravity.z));
		
		sDebugDraw.setDebugMode(sDebugDraw.DBG_DrawWireframe);
		m_dynamicsWorld->setDebugDrawer(&sDebugDraw);
		initialized=true;

	}
	
 
	void CPhysicsManager::Tick()
	{		
		//The first arg is delta in seconds		
		m_dynamicsWorld->stepSimulation(Game->getDelta(),3);

	}






	

	bool CPhysicsManager::isInitialized()
	{
		return initialized;
	}
 


	void CPhysicsManager::addConvexVerticesCollider(btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation)
	{
			///like adding a collision Triggering (as example)			
			if (vertices.size() > 0 && isEntity==false)
			{
				float mass = 0.f;
				btTransform startTransform;
				//can use a shift
				startTransform.setIdentity();
				startTransform.setOrigin(btVector3(0,0,0.f));
				//this create an internal copy of the vertices				
			 	 btCollisionShape* shape = new btConvexHullShape( &vertices[0].getX(),vertices.size());

				 m_collisionShapes.push_back(shape);

				
			 	localCreateRigidBody(mass, startTransform,shape);
			}

	}

	btRigidBody* CPhysicsManager::localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);

		if (isDynamic)shape->calculateLocalInertia(mass,localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		rbInfo.m_additionalDamping = true;
		btRigidBody* body = new btRigidBody(rbInfo);
		
		//Add this body to our world
		m_dynamicsWorld->addRigidBody(body,X::XCollisionGroups::WALLS,X::XCollidesWith::WITH_EVERYTHING);

	 	return body;
	}




	btDiscreteDynamicsWorld*  CPhysicsManager::getDynamic()
	{
		return m_dynamicsWorld;
	}


	void CPhysicsManager::setPhysicsDebugDrawing(bool state)
	{
		physicsDrawDebug=state;
	}

	bool CPhysicsManager::getPhysicsDebugDrawing()
	{
		return physicsDrawDebug;
	}


	void CPhysicsManager::helperRagDoll (const btVector3& positionOffset,btScalar scale_ragdoll)
	{


	}









}
