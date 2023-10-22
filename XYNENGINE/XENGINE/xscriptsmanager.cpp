//
#include "xmain.h" 
 
namespace X
{ 


        static void Vector3DefaultConstructor(float x, float y, float z,CVector3 *self)
       {
                new(self)  CVector3(x,y,z);
        }

        static void QuaternionDefaultConstructor(float x, float y, float z,float w,CQuaternion *self)
        {
                new(self)  CQuaternion(x,y,z,w);
        }

		void CEntity_Construct(CEntity *o)
		{
		  // Call the constructor to initialize the already allocated memory
		  new(o) CEntity;
		}

		void CEntity_Destruct(CEntity *o)
		{
		  // Call the destructor to release the memory that the members hold
		  o->~CEntity();
		}
 

		int mrand_XS(int a,int b)
		{
			return mrand(a,b);
		}

	 
		void XS_Sleep(int ms)
		{
			Sleep(ms);
		}


		//Fake LOG.write
		void dummy_Write(const std::string &in)
		{
			Game->Log()->write("<%s:%i %s> %s",__FILE__,__LINE__,__FUNCTION__,in.c_str());

		}
	 

	//Message callback PUMP, redirecting to engine log file.
	void MessageCallback(const asSMessageInfo *msg, void *param)
	{

		
		if( msg->type == asMSGTYPE_WARNING ) 
		{
			Game->Log()->write("<%s:%i %s> [%s] Warrning, %s (Line %i/%i)",__FILE__,__LINE__,__FUNCTION__,msg->section,msg->message,msg->row,msg->col);
		}
		else if( msg->type == asMSGTYPE_INFORMATION ) 
		{
			Game->Log()->write("<%s:%i %s> [%s] %s (Line %i/%i)",__FILE__,__LINE__,__FUNCTION__,msg->section,msg->message,msg->row,msg->col);
		}
		else if( msg->type == asMSGTYPE_ERROR ) 
		{
			Game->Log()->write("<%s:%i %s> [%s] Error, %s (Line %i/%i)",__FILE__,__LINE__,__FUNCTION__,msg->section,msg->message,msg->row,msg->col);			
		}

	}

	 
	CScriptsManager::CScriptsManager(){}

	CScriptsManager::~CScriptsManager()	
	{
		Game->Log()->write("<%s:%i %s> AngelScript deInitializing. ",__FILE__,__LINE__,__FUNCTION__);		
		ctx->Release();
		eScripting->Release();
	}





	bool CScriptsManager::Init()
	{
		Game->Log()->write("<%s:%i %s> AngelScript %s Initializing. ",__FILE__,__LINE__,__FUNCTION__,ANGELSCRIPT_VERSION_STRING);

		eScripting = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		if( eScripting == 0 )
		{
			Game->Log()->write("<%s:%i %s> AngelScript failed to initialize. ",__FILE__,__LINE__,__FUNCTION__);
			return false;
		}
		else
		{
			Game->Log()->write("<%s:%i %s> AngelScript Initialized. ",__FILE__,__LINE__,__FUNCTION__);
		}

		
		RegisterStdString(eScripting);

		Game->Log()->write("<%s:%i %s> AngelScript registered Std::String. ",__FILE__,__LINE__,__FUNCTION__);
	 	

		//Register Logging callback
		eScripting->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); 

		//Global functions
		eScripting->RegisterGlobalFunction("void Log(const string &in)", asFUNCTION(dummy_Write), asCALL_CDECL); 
		eScripting->RegisterGlobalFunction("int mrand(int,int)", asFUNCTIONPR(mrand_XS,(int,int),int), asCALL_CDECL); 
		eScripting->RegisterGlobalFunction("void Sleep(int)", asFUNCTIONPR(XS_Sleep,(int),void), asCALL_CDECL); 

		 
		eScripting->RegisterObjectType("CVector3",sizeof(CVector3),asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA);	
		eScripting->RegisterObjectProperty("CVector3", "float x", offsetof(CVector3, x));
		eScripting->RegisterObjectProperty("CVector3", "float y", offsetof(CVector3, y));
		eScripting->RegisterObjectProperty("CVector3", "float z", offsetof(CVector3, z));
		eScripting->RegisterObjectBehaviour("CVector3", asBEHAVE_CONSTRUCT,  "void f(float, float, float)",asFUNCTION(Vector3DefaultConstructor),  asCALL_CDECL_OBJLAST); 
		eScripting->RegisterObjectMethod("CVector3", "CVector3 opAdd(const CVector3 &in)",  asMETHODPR(CVector3, operator+, (const CVector3&), CVector3), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CVector3", "CVector3 opSub(const CVector3 &in)",  asMETHODPR(CVector3, operator-, (const CVector3&), CVector3), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CVector3", "CVector3 opMul(const CVector3 &in)",  asMETHODPR(CVector3, operator*, (const CVector3&), CVector3), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CVector3", "float Distance(const CVector3 &in, const CVector3 &in)",  asMETHODPR(CVector3, Distance, (const CVector3&,const CVector3&),float), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CVector3", "CVector3 opMul(float)",  asMETHODPR(CVector3, operator*, (float), CVector3), asCALL_THISCALL); 

	 
		eScripting->RegisterObjectType("CQuaternion",sizeof(CQuaternion),asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA);	
		eScripting->RegisterObjectProperty("CQuaternion", "float x", offsetof(CQuaternion, x));
		eScripting->RegisterObjectProperty("CQuaternion", "float y", offsetof(CQuaternion, y));
		eScripting->RegisterObjectProperty("CQuaternion", "float z", offsetof(CQuaternion, z));
		eScripting->RegisterObjectProperty("CQuaternion", "float w", offsetof(CQuaternion, w));
		eScripting->RegisterObjectBehaviour("CQuaternion", asBEHAVE_CONSTRUCT,  "void f(float, float, float, float)",asFUNCTION(QuaternionDefaultConstructor),  asCALL_CDECL_OBJLAST); 

		//As the CEntity struct contains std::strings, you must not register it with asOBJ_POD. 
		//You must also register the asBEHAVE_CONSTRUCT and asBEHAVE_DESTRUCT behaviours for the 
		//type to properly initialize the members and release the memory of the strings when the object is destroyed.
		//you need to have an assignment too
		eScripting->RegisterObjectType("CEntity",sizeof(CEntity), asOBJ_VALUE | asOBJ_APP_CLASS  );    
 		eScripting->RegisterObjectProperty("CEntity", "string classname", offsetof(CEntity, classname));
		eScripting->RegisterObjectProperty("CEntity", "string model_name",	offsetof(CEntity, model_name));
		eScripting->RegisterObjectProperty("CEntity", "string entity_name",	 offsetof(CEntity, entity_name));
		eScripting->RegisterObjectProperty("CEntity", "string targetname",	 offsetof(CEntity, targetname));
		eScripting->RegisterObjectProperty("CEntity", "string target",	 offsetof(CEntity, target));
		eScripting->RegisterObjectProperty("CEntity", "CVector3 origin", offsetof(CEntity, origin));
		eScripting->RegisterObjectProperty("CEntity", "CVector3 color", offsetof(CEntity, color));
		eScripting->RegisterObjectProperty("CEntity", "CVector3 angle", offsetof(CEntity, angle));
		eScripting->RegisterObjectProperty("CEntity", "float radius", offsetof(CEntity, radius));
		eScripting->RegisterObjectProperty("CEntity", "float weight", offsetof(CEntity, weight));
		eScripting->RegisterObjectProperty("CEntity", "int fov",		offsetof(CEntity, fov));
		eScripting->RegisterObjectProperty("CEntity", "bool castshadows", offsetof(CEntity, castshadows));
		eScripting->RegisterObjectProperty("CEntity", "bool bindable", offsetof(CEntity, bindable));
		eScripting->RegisterObjectProperty("CEntity", "bool switchable", offsetof(CEntity, switchable));
		eScripting->RegisterObjectProperty("CEntity", "bool bCharacter", offsetof(CEntity, bCharacter));
		eScripting->RegisterObjectProperty("CEntity", "float time", offsetof(CEntity, time));
		eScripting->RegisterObjectBehaviour("CEntity", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(CEntity_Construct), asCALL_CDECL_OBJLAST);
        eScripting->RegisterObjectBehaviour("CEntity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(CEntity_Destruct), asCALL_CDECL_OBJLAST);
 		eScripting->RegisterObjectMethod("CEntity", "CEntity &opAssign(const CEntity &in)", asMETHODPR(CEntity, operator =, (const CEntity&), CEntity&), asCALL_THISCALL);
 

		//ENUMS
		eScripting->RegisterEnum("LightType");
		eScripting->RegisterEnumValue("LightType","XLIGHT_NONE",0);
		eScripting->RegisterEnumValue("LightType","XLIGHT_OMNI",1);
		eScripting->RegisterEnumValue("LightType","XLIGHT_SPOT",2);
		eScripting->RegisterEnumValue("LightType","XLIGHT_TYPES",3);		

		eScripting->RegisterEnum("CameraModes");
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_ISOMETRIC",0);
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_CUTSCENE",1);
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_FREE",2);
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_PLATFORM",3);
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_STATIC",4);
		eScripting->RegisterEnumValue("CameraModes","CAMERA_MODE_COUNT",5);
	 
		eScripting->RegisterEnum("eActorType");
		eScripting->RegisterEnumValue("eActorType","ACTORTYPE_PLAYER",0);
		eScripting->RegisterEnumValue("eActorType","ACTORTYPE_OBJECT",1);
		eScripting->RegisterEnumValue("eActorType","ACTORTYPE_UNKNOWN",2);

		eScripting->RegisterEnum("eActorWalkDirection");
		eScripting->RegisterEnumValue("eActorWalkDirection","ACTORWALK_FORWARD",0);
		eScripting->RegisterEnumValue("eActorWalkDirection","ACTORWALK_BACKWARD",1);
		eScripting->RegisterEnumValue("eActorWalkDirection","ACTORWALK_STOP",2);

		eScripting->RegisterEnum("eActorRotationDirection");
		eScripting->RegisterEnumValue("eActorRotationDirection","ACTORROTATE_LEFT",0);
		eScripting->RegisterEnumValue("eActorRotationDirection","ACTORROTATE_RIGHT",1);
		eScripting->RegisterEnumValue("eActorRotationDirection","ACTORROTATE_NONE",2);

		 
		eScripting->RegisterEnum("XKeyState");
		eScripting->RegisterEnumValue("XKeyState","UP",0);
		eScripting->RegisterEnumValue("XKeyState","DOWN",1);
		eScripting->RegisterEnumValue("XKeyState","KEYSTATES_TOTAL",2);

		eScripting->RegisterEnum("XKeys");
		eScripting->RegisterEnumValue("XKeys","RIGHTARROW",0);
		eScripting->RegisterEnumValue("XKeys","LEFTARROW",1);				
		eScripting->RegisterEnumValue("XKeys","UPARROW",2);
		eScripting->RegisterEnumValue("XKeys","DOWNARROW",3);
		eScripting->RegisterEnumValue("XKeys","KEYW",4);
		eScripting->RegisterEnumValue("XKeys","KEYS",5);
		eScripting->RegisterEnumValue("XKeys","KEYA",6);
		eScripting->RegisterEnumValue("XKeys","KEYD",7);
		eScripting->RegisterEnumValue("XKeys","SPACE",8);
		eScripting->RegisterEnumValue("XKeys","ENTER",9);
		eScripting->RegisterEnumValue("XKeys","KEYF1",10);
		eScripting->RegisterEnumValue("XKeys","KEYF2",11);
		eScripting->RegisterEnumValue("XKeys","KEYF3",12);
		eScripting->RegisterEnumValue("XKeys","KEYF4",13);
		eScripting->RegisterEnumValue("XKeys","KEYF5",14);
		eScripting->RegisterEnumValue("XKeys","KEYF6",15);
		eScripting->RegisterEnumValue("XKeys","KEYF7",16);
		eScripting->RegisterEnumValue("XKeys","KEYF8",17);
		eScripting->RegisterEnumValue("XKeys","KEYF9",18);
		eScripting->RegisterEnumValue("XKeys","KEYF10",19);
		eScripting->RegisterEnumValue("XKeys","KEYF11",20);
		eScripting->RegisterEnumValue("XKeys","KEYF12",21);
		eScripting->RegisterEnumValue("XKeys","KEYZ",22);
		eScripting->RegisterEnumValue("XKeys","KEYX",23);
		eScripting->RegisterEnumValue("XKeys","KEYC",24);
 
	

  

		//Register all engine data.	 
		 
		eScripting->RegisterObjectType("CCamera", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CCamera", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(CCamera,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CCamera", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(CCamera,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CCamera", "CVector3 Position()",				asMETHODPR(CCamera,Position, (), CVector3), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void bindTo(const string &in)",		asMETHODPR(CCamera,bindTo, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void unbind()",						asMETHODPR(CCamera,unbind, (), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setFOV(float,bool,float)",					asMETHODPR(CCamera,setFOV, (float,bool,float), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setFOV(float)",							asMETHODPR(CCamera,setFOV, (float), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "float getFOV()",					asMETHODPR(CCamera,getFOV, (), float), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "bool isBound()",						asMETHODPR(CCamera,isBound, (),bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setMode(CameraModes c_Mode)",			asMETHODPR(CCamera,setMode, (CameraModes), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setPosition(const CVector3 &in,CVector3 &in,CVector3 &in)",			asMETHODPR(CCamera,setPosition, (const CVector3&,const CVector3&,const CVector3&), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setView(const CVector3 &in)",			asMETHODPR(CCamera,setView, (const CVector3&), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setPosition(const CVector3 &in)",			asMETHODPR(CCamera,setPosition, (const CVector3&), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setUpVector(const CVector3 &in)",			asMETHODPR(CCamera,setUpVector, (const CVector3&), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "void setViewTarget(const string &in)",			asMETHODPR(CCamera,setViewTarget, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CCamera", "bool setFromEntity(const CEntity &in)",			asMETHODPR(CCamera,setFromEntity, (const CEntity &in), bool), asCALL_THISCALL); 
		eScripting->RegisterGlobalProperty("CCamera Camera", Game->Camera());  

 
		eScripting->RegisterObjectType("XBSP", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("XBSP", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(XBSP,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("XBSP", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(XBSP,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("XBSP", "bool loadMap(const string &in)",		  asMETHODPR(XBSP,loadMap, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterGlobalProperty("XBSP Level", Game->Level());  


		eScripting->RegisterObjectType("CEntitiesManager", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CEntitiesManager", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(CEntitiesManager,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CEntitiesManager", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(CEntitiesManager,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CEntitiesManager", "bool getEntityClass(const string &in, CEntity &out)",		  asMETHODPR(CEntitiesManager,XS_getEntityClass, (const std::string &in,CEntity &out), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CEntitiesManager", "bool getEntityName(const string &in, CEntity &out)",		  asMETHODPR(CEntitiesManager,XS_getEntityName, (const std::string &in,CEntity &out), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CEntitiesManager", "bool getEntityTargetName(const string &in, CEntity &out)",		  asMETHODPR(CEntitiesManager,XS_getEntityTargetName, (const std::string &in,CEntity &out), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CEntitiesManager", "bool getEntityTarget(const string &in, CEntity &out)",		  asMETHODPR(CEntitiesManager,XS_getEntityTarget, (const std::string &in,CEntity &out), bool), asCALL_THISCALL); 


		eScripting->RegisterGlobalProperty("CEntitiesManager EntitiesManager", Game->EntitiesManager());  

		 
		eScripting->RegisterObjectType("CPhysicsManager", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CPhysicsManager", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(CPhysicsManager,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CPhysicsManager", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(CPhysicsManager,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CPhysicsManager", "void Init(const CVector3 &in,CVector3 &in,CVector3 &in)",		  asMETHODPR(CPhysicsManager,Init, (const CVector3&,const CVector3&,const  CVector3&), void), asCALL_THISCALL); 
		eScripting->RegisterGlobalProperty("CPhysicsManager Physics", Game->Physics());  

		 
		eScripting->RegisterObjectType("CLightsManager", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CLightsManager", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(CLightsManager,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CLightsManager", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(CLightsManager,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CLightsManager", "int setLightRadius(int,float)",		  asMETHODPR(CLightsManager,setLightRadius, (int,float ), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CLightsManager", "int createLight(const CVector3 &in, const CVector3 &in, const CVector3 &in,LightType l_type,float,bool)",		  asMETHODPR(CLightsManager,XS_createLight, (const CVector3 &position,const CVector3 &color_ambient,const CVector3 &color_diffuse,LightType,float ,bool ), int), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CLightsManager", "int bindLightToActorBone(int,const string &in,const string &in)",		  asMETHODPR(CLightsManager,XS_bindLightToActorBone, (int  ,const std::string &actorName,const std::string &boneName), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CLightsManager", "bool deleteLight(int)",		  asMETHODPR(CLightsManager,XS_deleteLight, (int),bool), asCALL_THISCALL); 
		eScripting->RegisterGlobalProperty("CLightsManager LightsManager", Game->LightsManager());  

	 
	 
		eScripting->RegisterObjectType("CActorsManager", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CActorsManager", asBEHAVE_ADDREF, "void cmar()	",							asMETHOD(CActorsManager,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CActorsManager", asBEHAVE_RELEASE, "void cmr()	",							asMETHOD(CActorsManager,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CActorsManager", "void Walk(const string &in, int,float,float )",				asMETHODPR(CActorsManager,XS_Walk, (const std::string &in, int,float,float), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void Rotate(const string &in, int,float )",					asMETHODPR(CActorsManager,XS_Rotate, (const std::string &in, int,float), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void SetAnimation(const string &in,const string &in )",		asMETHODPR(CActorsManager,XS_SetAnimation, (const std::string &in,const std::string &animin), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void RotateTo(const string &in,const string &in )",			asMETHODPR(CActorsManager,XS_RotateTo, (const std::string &in,const std::string &toactor), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void Bind(const string &in,const string &in,const string &in ,const string &in  )",		asMETHODPR(CActorsManager,XS_Bind, (const std::string &in,const std::string &mybone,const std::string &toactor,const std::string &toactorbone), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void Activate(const string &in)",							asMETHODPR(CActorsManager,XS_Activate, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void ClearVelocities(const string &in)",						asMETHODPR(CActorsManager,XS_ClearVelocities, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void SetPosition(const string &in,const CVector3 &in)",		asMETHODPR(CActorsManager,XS_SetPosition, (const std::string &in,const CVector3 &position), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void GetPosition(const string &in,CVector3 &out)",			asMETHODPR(CActorsManager,XS_GetPosition, (const std::string &in,CVector3 &out), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void GetEntity(const string &in,CEntity &out)",				asMETHODPR(CActorsManager,XS_GetEntity, (const std::string &in,CEntity &out), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void DeleteActor(const string &in)",							asMETHODPR(CActorsManager,XS_DeleteActor, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void SetMass(const string &in,float)",						asMETHODPR(CActorsManager,XS_SetMass, (const std::string &in,float fMass), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "float GetMass(const string &in)",							asMETHODPR(CActorsManager,XS_GetMass, (const std::string &in), float), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void AddWaypoint(const string &in,const string &in)",		asMETHODPR(CActorsManager,XS_AddWaypoint, (const std::string &actorName,const std::string &uniqueEntityName), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void ClearWaypoints(const string &in)",						asMETHODPR(CActorsManager,XS_ClearWaypoints, (const std::string &actorName), void), asCALL_THISCALL); 

		eScripting->RegisterGlobalProperty("CActorsManager ActorsManager", Game->ActorsManager());  

	/*
		eScripting->RegisterObjectMethod("CActorsManager", "bool createActor(const string &in,const string &in,const CVector3 &in,eActorType )",		  asMETHODPR(CActorsManager,XS_createActor, (const std::string &ActorName,const std::string &ModelName,const CVector3 &origin,eActorType eActorType), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void jumpActor(const string &in)",		  asMETHODPR(CActorsManager,XS_Jump, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void stopActor(const string &in)",		  asMETHODPR(CActorsManager,XS_Stop, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void walkActor(const string &in, int )",		  asMETHODPR(CActorsManager,XS_Walk, (const std::string &in, int), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "int getAnimationFrame(const string &in )",		  asMETHODPR(CActorsManager,XS_getAnimationFrame, (const std::string &in), int), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "bool isOnGround(const string &in )",		  asMETHODPR(CActorsManager,XS_isOnGround, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "bool isBound(const string &in)",		  asMETHODPR(CActorsManager,XS_isBound, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void rotateActor(const string &in, eActorRotationDirection rotate)",		  asMETHODPR(CActorsManager,XS_Rotate, (const std::string &in,eActorRotationDirection), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void rotateActorToAngle(const string &in, const CVector3 &in)",		  asMETHODPR(CActorsManager,XS_rotateActorToAngle, (const std::string &in,const CVector3 &inv), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "bool bindActorToActorBone(const string &in,const string &in,const string &in,const CVector3 &in)",		  asMETHODPR(CActorsManager,XS_bindActorToActorBone, (const std::string &in,const std::string &din,const std::string &bin,const CVector3 &offset), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void unbindActor(const string &in)",		  asMETHODPR(CActorsManager,XS_unbindActor, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "CVector3 getActorPosition(const string &in)",		  asMETHODPR(CActorsManager,XS_getActorPosition, (const std::string &in), CVector3), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void moveActorToPosition(const string &in, const CVector3 &in)",		  asMETHODPR(CActorsManager,XS_moveActorToPosition, (const std::string &in, const CVector3 &inv), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void getBoundData(const string &in,string &out, string &out)",		  asMETHODPR(CActorsManager,XS_getBoundData, (const std::string &in, std::string &aout,std::string &bout), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void setActorMass(const string &in,float)",		  asMETHODPR(CActorsManager,XS_setActorMass, (const std::string &in, float), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "float getActorMass(const string &in)",		  asMETHODPR(CActorsManager,XS_getActorMass, (const std::string &in), float), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void deleteActor(const string &in)",		  asMETHODPR(CActorsManager,XS_deleteActor, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "bool isActorExisting(const string &in)",		  asMETHODPR(CActorsManager,XS_isActorExisting, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void getActorBoundTo(const string &in,string &in,string &out)",		  asMETHODPR(CActorsManager,XS_getActorBoundTo, (const std::string &in,const std::string &inn, std::string &out), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "bool isAnimated(const string &in)",		  asMETHODPR(CActorsManager,XS_isAnimated, (const std::string &in), bool), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void activateActor(const string &in)",		  asMETHODPR(CActorsManager,XS_activateActor, (const std::string &in), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void rotateActorTo(const string &in,const string &in )",		  asMETHODPR(CActorsManager,XS_rotateActorTo, (const std::string &in,const std::string &tin), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void findClosestActorTo(const string &in,string &out)",		  asMETHODPR(CActorsManager,XS_findClosestActor, (const std::string &in, std::string &out), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void findClosestActorWithFlags(const string &in,string &out,const tActorFlags &in )",		  asMETHODPR(CActorsManager,XS_findClosestActorWithFlags, (const std::string &in,std::string &out,const X::tActorFlags &inf), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void getActorFlags(const string &in,tActorFlags &out)",		  asMETHODPR(CActorsManager,XS_getActorFlags, (const std::string &in,tActorFlags out), void), asCALL_THISCALL); 
		eScripting->RegisterObjectMethod("CActorsManager", "void setActorFlags(const string &in,const tActorFlags &in)",		  asMETHODPR(CActorsManager,XS_setActorFlags, (const std::string &in,const tActorFlags &flagsin), void), asCALL_THISCALL); 
		eScripting->RegisterGlobalProperty("CActorsManager ActorsManager", Game->ActorsManager());  
		 */
		eScripting->RegisterObjectType("CEffectsManager", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("CEffectsManager", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(CEffectsManager,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("CEffectsManager", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(CEffectsManager,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("CEffectsManager", "void removeQueuedEffects()",		  asMETHODPR(CEffectsManager,removeQueuedEffects, (void), void), asCALL_THISCALL); 		
		eScripting->RegisterObjectMethod("CEffectsManager", "void removeEffect(const string &in)",		  asMETHODPR(CEffectsManager,removeEffect, (const std::string &in), void), asCALL_THISCALL); 		
		eScripting->RegisterObjectMethod("CEffectsManager", "bool addEffect(const string &in)",		  asMETHODPR(CEffectsManager,addEffect, (const std::string &in), bool), asCALL_THISCALL); 		
		eScripting->RegisterObjectMethod("CEffectsManager", "bool loadEffect(const string &in)",		  asMETHODPR(CEffectsManager,loadEffect, (const std::string &in), bool), asCALL_THISCALL); 		
		eScripting->RegisterObjectMethod("CEffectsManager", "bool loadEffect(const string &in, const string &in, const string &in)",		  asMETHODPR(CEffectsManager,loadEffect, (const std::string &s_EffectName,const std::string &s_optFragmentShader, const std::string &s_optVertexShader), bool), asCALL_THISCALL); 		
		eScripting->RegisterGlobalProperty("CEffectsManager EffectsManager", Game->EffectsManager());  

		
		 
		eScripting->RegisterObjectType("Engine", 0, asOBJ_REF); 
		eScripting->RegisterObjectBehaviour("Engine", asBEHAVE_ADDREF, "void cmar()	",    asMETHOD(Engine,AddRef),        asCALL_THISCALL);
		eScripting->RegisterObjectBehaviour("Engine", asBEHAVE_RELEASE, "void cmr()	",    asMETHOD(Engine,Release),        asCALL_THISCALL);
		eScripting->RegisterObjectMethod("Engine", "void Quit()",						 asMETHODPR(Engine,Quit, (void),void), asCALL_THISCALL); 	
		eScripting->RegisterObjectMethod("Engine", "XKeyState getKeyState(XKeys k_Key)",	 asMETHODPR(Engine,getKeyState, (XKeys k_Key),XKeyState), asCALL_THISCALL); 	
		eScripting->RegisterObjectMethod("Engine", "float getCutSceneTimer()",				asMETHODPR(Engine,getCutSceneTimer, (void),float), asCALL_THISCALL); 	

		eScripting->RegisterGlobalProperty("Engine Game", Game);  
		  
		return true;
  
	}

	int CScriptsManager::Run(const std::string &funcName)
	{	
		char fName[128];
		sprintf(fName,"%s",funcName.c_str());

			if( ctx->Prepare(eScripting->GetModule(0)->GetFunctionIdByName(funcName.c_str())) < 0 ) 
			{
				Game->Log()->write("<%s:%i %s> Error, couldn't prepare function \"%s\"",__FILE__,__LINE__,__FUNCTION__,fName);
				ctx->Release();
				eScripting->Release();
				return false;
			}

		return ctx->Execute();
		
	}


	bool CScriptsManager::LoadScript(const std::string &xs_ScriptName,const std::string &xs_ModuleName)
	{
				Game->Log()->write("<%s:%i %s> \"%s\", module: %s. ",__FILE__,__LINE__,__FUNCTION__,xs_ScriptName.c_str(),xs_ModuleName.c_str());
				// Read the entire script file
				FILE * pFile = fopen(xs_ScriptName.c_str(), "rb");
				if( pFile == 0 )
				{
					Game->Log()->write("<%s:%i %s> Error, \"%s\" not found. ",__FILE__,__LINE__,__FUNCTION__,xs_ScriptName.c_str());
					return false;
				}

				// Find the length of the script file
				fseek(pFile, 0, SEEK_END);
				int len = ftell(pFile);
				fseek(pFile, 0, SEEK_SET);

				// On Win32 it is possible to do the following instead
				// int len = _filelength(_fileno(f));

				std::string code;
				code.resize(len);

				// Read length of script file
				size_t c = fread(&code[0], len, 1, pFile);
				fclose(pFile);

				if( c == 0 ) 
				{
					Game->Log()->write("<%s:%i %s> Error, Failed to load script file.",__FILE__,__LINE__,__FUNCTION__);
					return false;
				}

				// Give the code to the script engine
				asIScriptModule *mod = eScripting->GetModule(0, asGM_ALWAYS_CREATE);

				int nRet = mod->AddScriptSection(xs_ModuleName.c_str(),code.c_str(),len,0);
				if( nRet < 0 ) 
				{
					Game->Log()->write("<%s:%i %s> Error, during AddScriptSection.",__FILE__,__LINE__,__FUNCTION__);
				}

				// At this point the engine has copied the code to an 
				// internal buffer so we are free to release the memory we 
				// allocated. 
				if(mod->Build()<0)
				{
					Game->Log()->write("<%s:%i %s> Error, build failed.",__FILE__,__LINE__,__FUNCTION__);
					return false;
				}
				 
	
				// We can also add other script sections if we would like.
				// All script sections will be compiled together as if they
				// where one large script. 
				Game->Log()->write("<%s:%i %s> Build ok.",__FILE__,__LINE__,__FUNCTION__);








			Game->Log()->write("<%s:%i %s> Creating context.",__FILE__,__LINE__,__FUNCTION__);
			ctx = eScripting->CreateContext();
			if( ctx == 0 ) 
			{
				Game->Log()->write("<%s:%i %s> Error, Context cannot be created. ",__FILE__,__LINE__,__FUNCTION__);
				return false;
			}

				
 /*
			Game->Log()->write("<%s:%i %s> Looking for main().",__FILE__,__LINE__,__FUNCTION__);
			int funcId = eScripting->GetModule(0)->GetFunctionIdByDecl("void main()");
			if( funcId < 0 )
			{
				Game->Log()->write("<%s:%i %s> Error, main() not found ",__FILE__,__LINE__,__FUNCTION__);
				return false;
			}
		
			Game->Log()->write("<%s:%i %s> Preparing function.",__FILE__,__LINE__,__FUNCTION__);
			if( ctx->Prepare(funcId) < 0 ) 
			{
				Game->Log()->write("<%s:%i %s> Error, couldn't prepare function ",__FILE__,__LINE__,__FUNCTION__);
				ctx->Release();
				eScripting->Release();
				return false;
			}
			*/

			
		 Game->Log()->write("<%s:%i %s> Finished loading script file.",__FILE__,__LINE__,__FUNCTION__);

				return true;
		
	}

	

	asIScriptEngine *CScriptsManager::Get()
	{
		return eScripting;
	}

}