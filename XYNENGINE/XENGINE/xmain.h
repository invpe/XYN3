#ifndef __XMAIN_H
#define __XMAIN_H

#pragma warning(disable:4786) // 255 char id truncation
#pragma warning(disable:4018) // Signed/unsigned mismatch
#pragma warning(disable:4244) // Conversion with possible loss of data
#pragma warning(disable:4503) // Decorated name length exceeded


#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"nvtristrip")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glee.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"jpeg.lib")
#pragma comment(lib,"bulletdynamics.lib")
#pragma comment(lib,"bulletcollision.lib")
#pragma comment(lib,"LinearMath.lib")
#ifdef _DEBUG
#pragma comment(lib,"angelscriptd.lib")
#pragma comment(lib,"xguid.lib")
#else
#pragma comment(lib,"angelscript.lib")
#pragma comment(lib,"xgui.lib")
#endif



//@@PHYSICS
#include "systems/bullet/btBulletCollisionCommon.h"			//INCLUDED 3MAY11
#include "systems/bullet/btBulletDynamicsCommon.h"				//INCLUDED 3MAY11
#include "systems/bullet/LinearMath/btGeometryUtil.h"
#include "systems/bullet/LinearMath/btVector3.h"
#include "systems/bullet/BulletDynamics/Character/btKinematicCharacterController.h"
#include "systems/bullet/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "GLDebugDrawer.h"
class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btCharacterControllerInterface;
class btDynamicCharacterController;
class btKinematicCharacterController;

//@@STD
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <fstream>
#include <cmath>
#include <time.h>
#include <map> 



#include "systems\gl\glee.h"
#include "systems\gl\gl.h"
#include "systems\gl\glut.h"
#include "systems\gl\glu.h"
#include "systems\gl\wglext.h"	 

 

//VSYNCOFF
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );

//VBO Buffer offsetting
#define BUFFER_OFFSET(i) ((char *)NULL + (i))		



//Xengine specific includes
#include "systems/as/angelscript.h"
#include "systems/as/scriptstdstring/scriptstdstring.h"


#include "xlog.h"
#include "xgui/gui.h"
#include "xstatesmanager.h"
#include "mathlib.h"
#include "xphysics.h"
#include "xfrustum.h"
#include "xtexturesmanager.h"
#include "xshader.h"
#include "xshadersmanager.h"
#include "xeffectsmanager.h"
#include "xmaterialsmanager.h"
#include "xyn.h"	
#include "xbsp.h"
#include "xentitiesmanager.h"
#include "xactorsmanager.h"
#include "xcamera.h"
#include "xlightsmanager.h"
#include "xbuffersmanager.h"
#include "xscriptsmanager.h"
#include "xTimer.h"
#include "xengine.h"

extern X::Engine *Game;

#endif