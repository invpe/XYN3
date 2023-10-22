#include "xmain.h"
#include "systems/gl/WGL_ARB_multisample.h"


namespace X
{
	/*
	*	Method: Constructor
	*/
	Engine::Engine()	
	{ 
				
			/*
			*	Obtain pointers to classes
			*/									 
			eLog			=	new X::CLogManager();	
			eStateManager	=	new X::CStateManager();
			eTexturesManager=	new X::CTexturesManager();
			eCamera			=	new X::CCamera();
			eLightsManager	=	new	X::CLightsManager();
			eActorsManager	=	new X::CActorsManager();
			ePhysics		=	new X::CPhysicsManager();			
			eFrustum		=	new X::CFrustum();				
			eFBOManager		=	new X::CBuffersManager();						
			eScripting		=	new X::CScriptsManager();
			eTimer			=	new XTimer::CTimer();
			eEffectsManager	=	new X::CEffectsManager();
			eMaterialsManager=  new X::CMaterialsManager();
			eShadersManager	=	new X::CShadersManager();
			eBsp			=	new X::XBSP();
			eEntitiesManager=	new	X::CEntitiesManager();
	}
	 
	GLvoid Engine::Viewport(CVector2 size)
	{
		glViewport(0, 0, (int)size.x, (int)size.y);
	}



	/*
	*	getMap()
	*	arguments	-
	*	synopsis	-	gets bsp class pointer to loaded map
	*/
	XBSP *Engine::Level()
	{
		 return eBsp;
		 
	}



	/*
	*	Method: overloadedConstructor - arguments
	*/
	void Engine::Init(int screenWidth, int screenHeight, char *title,WNDPROC procWnd,HICON icon)
	{

			
		 
			 
			modelsLoaded=false;
			guiLoaded=false;
	 

			for(int a=0;a<KEYS_TOTAL;a++)
			s_KeyStates[a]=UP;

			i_Application=icon;
		

			eLog->write("<%s:%i %s> XENGINE Warming Up",__FILE__,__LINE__,__FUNCTION__);

			if(createWindow(title,screenWidth,screenHeight,NULL,false,procWnd))
			{

				eLog->write("<%s:%i %s> Window obtained, calling gl defaults. ",__FILE__,__LINE__,__FUNCTION__);



				if(initGL()==false)
				{
					
					eLog->write("<%s:%i %s> Fatal error in postLoading phase. ",__FILE__,__LINE__,__FUNCTION__);
					exit(0);
				}
			
			}
			else
			{
				eLog->write("<%s:%i %s> window creation failed, boiling out... ",__FILE__,__LINE__,__FUNCTION__);
			}

			eTimer->start();
			
	}


	void Engine::Quit()
	{
		this->~Engine();
	}


	/*
	*	Method: deConstructor
	*/	
	Engine::~Engine()
	{
		eLog->write("<%s:%i %s>",__FILE__,__LINE__,__FUNCTION__);
		
		if(guiLoaded)
		{
			eGui->Closure();
			Gui::DestroyInstance( eGui );			
		}
		eScripting->~CScriptsManager();
		eActorsManager->~CActorsManager();
		eBsp->~XBSP();
		ePhysics->~CPhysicsManager();
		eEffectsManager->~CEffectsManager();	
	
		DestroyWindow(hwndMain);
		eLog->write("<%s:%i %s> XENGINE OUT. ",__FILE__,__LINE__,__FUNCTION__);		
		eLog->~CLogManager();
		exit(0);
	}




	/*
	*	Method: initGL()
	*	arguments	- 
	*	synopsis	- enables most of the GL stuff, synchronizes state manager
	*				- called when GL context is ready, and we can continue with
	*				- gl dependent initializations
	*/
	bool Engine::initGL()
	{



		//StateManager is GLContext dependent, so we inject it here.
		eLog->write("<%s:%i %s> StateManager Init",__FILE__,__LINE__,__FUNCTION__);
		
		eStateManager->glSynchronize();

		eStateManager->Set(BFCULLING,       ENABLED);
		eStateManager->Set(CULLSIDE,        CULLSIDE_FRONT);
		eStateManager->Set(FACEORIENTATION, FACEORIENTATION_CCW);
		eStateManager->Set(SHADEMODE,       SHADEMODE_GOURAUD);
		eStateManager->Set(POLYGONMODE,     POLYGONMODE_FILL);		
		eStateManager->Set(FOG,             DISABLED);
		eStateManager->Set(ZFUNC,           CMP_LESS);
 		eStateManager->Set(STENCILBUFFER,   DISABLED);
		eStateManager->Set(READBUFFER,      COLORBUFFER_BACK);
		eStateManager->Set(DRAWBUFFER,      COLORBUFFER_BACK);		
		eStateManager->Set(SRCBLEND,			BLEND_SRCALPHA);
		eStateManager->Set(DSTBLEND,			BLEND_INVSRCALPHA);
		eStateManager->Set(BLENDING,        DISABLED);
		eStateManager->Set(ZBUFFER,         ENABLED);
		eStateManager->Set(ZBUFFERWRITE,    ENABLED);
		 
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST); 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	 
		glPolygonOffset(1.0, 1.0);
   
		eLog->write("<%s:%i %s> StateManager Completed",__FILE__,__LINE__,__FUNCTION__);

		//GLfloat ambient[4] = {0.19225f, 0.19225f, 0.19225f, 1.0f};
		GLfloat ambient[4] = {0,0,0, 1.0f};
		//GLfloat diffuse[4] = {0.50754f, 0.50754f, 0.50754f, 1.0f};
		GLfloat diffuse[4] = {1,1,1, 1.0f};
		GLfloat specular[4] = {1,1,1, 1.0f};
		GLfloat emission[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat shininess = 120.0;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);	

		//Configure number of render buffers for frame rendering
		b_renderBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
		b_renderBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;


		//Create frame render GLList
		float p[2];
		l_FrameRenderList = glGenLists (1);
		glNewList(l_FrameRenderList, GL_COMPILE);
		glBegin(GL_QUADS);	
		p[0]=0;
		p[1]=1;
		glMultiTexCoord2fv(XTEXTURE0,p);
		glVertex2f(0,0);									
		p[0]=0;
		p[1]=0;
		glMultiTexCoord2fv(XTEXTURE0,p);
		glVertex2f(0,screenSize.y);									
		p[0]=1;
		p[1]=0;
		glMultiTexCoord2fv(XTEXTURE0,p);
		glVertex2f(screenSize.x,screenSize.y);								
		p[0]=1;
		p[1]=1;
		glMultiTexCoord2fv(XTEXTURE0,p);
		glVertex2f(screenSize.x,0);								
		glEnd();															
		glEndList();


		

					 
		//Shaders are GLContext dependent, so we inject it here.
		eLog->write("<%s:%i %s> Shaders init",__FILE__,__LINE__,__FUNCTION__);
		if(loadShaders()==false)
		return false;


		//Gui is GLContext dependent, so we inject it here.
		eLog->write("<%s:%i %s> GUI init",__FILE__,__LINE__,__FUNCTION__);

			

		eGui=Gui::CreateInstance();
		
		eGui->Initiation();
		 
		if(eGui->Settings((HWND)hwndMain)==false)
		{
			eLog->write("<%s:%i %s> GUI failed to initialize",__FILE__,__LINE__,__FUNCTION__);
			return false;
		}
		
	 
		/*Create depthcubemaps for lights*/
		eLightsManager->initializeMaps();



		/*Create the FRAME buffer*/
		//We render to two color buffers as we have two textures and 1 depth texture
		//t_FrameTextureOriginal - never modified
		//t_FrameTexture - modified with effects applied
		//t_FrameTextureDepth - depth texture 

		eFBOManager->Create("FRAME","FRAME",screenSize,GL_DEPTH_COMPONENT);
		t_FrameTexture=eTexturesManager->EmptyTexture(screenSize.x,screenSize.y);
		t_FrameTextureOriginal=eTexturesManager->EmptyTexture(screenSize.x,screenSize.y);		

			glGenTextures(1, &t_FrameTextureDepth);
			glBindTexture(GL_TEXTURE_2D, t_FrameTextureDepth);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT   , screenSize.x, screenSize.y, 0,  GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0); 		

		eFBOManager->AttachTexture("FRAME",GL_COLOR_ATTACHMENT1,t_FrameTextureOriginal,GL_TEXTURE_2D);
		eFBOManager->AttachTexture("FRAME",GL_COLOR_ATTACHMENT0,t_FrameTexture,GL_TEXTURE_2D);
		eFBOManager->AttachTexture("FRAME",GL_DEPTH_ATTACHMENT,t_FrameTextureDepth,GL_TEXTURE_2D);
		
		if(eFBOManager->CheckFrameBufferStatus()==false)
		{
			eLog->write("<%s:%i %s> Framebuffer error",__FILE__,__LINE__,__FUNCTION__);
			exit(0);
		}
 		 eFBOManager->Unbind("FRAME","FRAME");

		 
		// Cube shadowmap fbo - render to: eLightsManager->tDepthCubeMap[x]
		eFBOManager->Create("SHADOWMAPPING","",CVector2(512,512), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		//Attach for sanity check
		eFBOManager->AttachTexture("SHADOWMAPPING",GL_DEPTH_ATTACHMENT,eLightsManager->tDepthCubeMap[0],GL_TEXTURE_CUBE_MAP_POSITIVE_X);		
		if(eFBOManager->CheckFrameBufferStatus()==false)
		{
			eLog->write("<%s:%i %s> Framebuffer error",__FILE__,__LINE__,__FUNCTION__);
				exit(0);
		}
 		eFBOManager->Unbind("SHADOWMAPPING","");
		
		  
		 
		return true;

	}


 

	/*
	*	createWindow( LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen,WNDPROC procWnd)
	*	arguments	-  
	*	synopsis	-  
	*/
	HWND Engine::createWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen,WNDPROC procWnd)
	{
	 
		 
		eLog->write("<%s:%i %s> %i x %i \"%s\" ",__FILE__,__LINE__,__FUNCTION__,width,height,strWindowName);
		memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
		wndclass.style = CS_HREDRAW | CS_VREDRAW;			// Regular drawing capabilities
		wndclass.lpfnWndProc = procWnd;						
		wndclass.hInstance = hInstance;						// Assign our hInstance
		wndclass.hIcon =   i_Application;	// General icon
		wndclass.hCursor = LoadCursor (NULL, IDC_ARROW); 		// An arrow for the cursor
		wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); ;	// A white window
		wndclass.lpszClassName = "x";			// Assign the class name

		if(!RegisterClass(&wndclass))
		{							// Register the class
			eLog->write("<%s:%i %s> Class registration failed ",__FILE__,__LINE__,__FUNCTION__);
			exit(0);
		
		} 
		RECT rWindow;
		rWindow.left	= 0;								// Set Left Value To 0
		rWindow.right	= width;							// Set Right Value To Requested Width
		rWindow.top	    = 0;								// Set Top Value To 0
		rWindow.bottom	= height;							// Set Bottom Value To Requested Height
		hwndMain = CreateWindow("x", strWindowName,  WS_POPUP | WS_BORDER |WS_SYSMENU , 0, 0,
							rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
							NULL, NULL, hInstance, NULL);

		hdcMain=GetDC(hwndMain);
		AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size
		if(!hwndMain||!hdcMain)
		{
			eLog->write("<%s:%i %s> Failed to obtain hwnd/hdc ",__FILE__,__LINE__,__FUNCTION__);
			exit(0); 
		}

		screenSize.x=width;
		screenSize.y=height;


		eLog->write("<%s:%i %s> PFD init ",__FILE__,__LINE__,__FUNCTION__);
		PIXELFORMATDESCRIPTOR pfd = {0};
		int pf = 0;
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		ChooseBestAntiAliasingPixelFormat(pf);			//ZJADA NAM FPSY!! JAKO OPCJE DAC

		//MSAA - na nvidiach < 8
		//CSAA - na nvidiach >= 8
		//ChooseAntiAliasingPixelFormat(pf,4);				//to dziala przy wykorzystaniu renderu na screen nie offscreen do fbo	FSAA po prostu
	 	// pf = ChoosePixelFormat(hdcMain, &pfd);
	 
		
		

		if (!SetPixelFormat(hdcMain, pf, &pfd))
		{
		 	eLog->write("<%s:%i %s> PFD failed ",__FILE__,__LINE__,__FUNCTION__);
			exit(0);

		}

		if (!(hglrc1 = wglCreateContext(hdcMain)))
		{
		 	eLog->write("<%s:%i %s> GL Context Failed ",__FILE__,__LINE__,__FUNCTION__);
			exit(0);
	 
		}

		if (!wglMakeCurrent(hdcMain, hglrc1))
		{
		  	eLog->write("<%s:%i %s> MakeCurrent failed ",__FILE__,__LINE__,__FUNCTION__);
			exit(0);

		}


		
	 
	 	//VSYNC OFF		 		 
	
		eLog->write("<%s:%i %s> Forced VSYNC off ",__FILE__,__LINE__,__FUNCTION__);
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0; 
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
		if( wglSwapIntervalEXT )
		wglSwapIntervalEXT(0); 
		eLog->write("<%s:%i %s> Main window up ",__FILE__,__LINE__,__FUNCTION__);
		 

    HWND hwndOwner;
	RECT rc, rc2;
	int	x,y; 
	/* Get the owner window and dialog box rectangles. */
	if((hwndOwner=GetParent(hwndMain))==NULL) hwndOwner = GetDesktopWindow();
	GetWindowRect(hwndOwner, &rc);
	GetWindowRect(hwndMain, &rc2);
	x = ((rc.right-rc.left) - (rc2.right-rc2.left)) / 2 +rc.left;
	y = ((rc.bottom-rc.top) - (rc2.bottom-rc2.top)) / 2 +rc.top;

	SetWindowPos(hwndMain,
		HWND_TOP,
		x, y,
		0, 0,		   /* ignores size arguments */
		SWP_NOSIZE);
 



		ShowWindow(hwndMain,SW_SHOW);

		
		return hwndMain;
	}

 

	/*
	*	renderCallback( void (*fct)())
	*	arguments	- function address
	*	synopsis	- can be used to pass a callback instead of using engine renderer
	*/
	GLvoid Engine::renderCallback( void (*fct)())
	{

		eLog->write("<%s:%i %s> Callback set. ",__FILE__,__LINE__,__FUNCTION__);
		renderCallbackFunc=fct;
	}





	X::CCamera	*Engine::Camera()
	{
		return eCamera;
	}

	X::CActorsManager *Engine::ActorsManager()
	{
		return eActorsManager;
	}

	X::CLightsManager *Engine::LightsManager()
	{
		return eLightsManager;
	}
	
	X::CPhysicsManager		*Engine::Physics()
	{
		return ePhysics;
	}

	X::CLogManager			*Engine::Log()
	{ 
		 
			return eLog;
		 
	}
	X::CTexturesManager		*Engine::TexturesManager()
	{
		return eTexturesManager;
	}
	X::CStateManager	*Engine::StateManager()
	{
		return eStateManager;
	}

	X::CFrustum				*Engine::Frustum()
	{
		return eFrustum;
	}
	X::CBuffersManager		*Engine::FBOManager()
	{
		return eFBOManager;
	}
	X::CEffectsManager		*Engine::EffectsManager()
	{
		return eEffectsManager;
	}
	X::CScriptsManager	*Engine::Scripting()
	{
		return eScripting;
	}
	X::CMaterialsManager *Engine::MaterialsManager()
	{
		return eMaterialsManager;
	}
	X::CShadersManager *Engine::ShadersManager()
	{
		return eShadersManager;
	}
	X::CEntitiesManager *Engine::EntitiesManager()
	{
		return eEntitiesManager;
	}



/*
	*	tick()
	*	arguments	-
	*	synsopsis	- performs per frame calculations
	*/
	bool Engine::tick()
	{
		static int lastFPS=0;
		static int lastFPSTick=0;
		static double lastTime=eTimer->getElapsedTimeInMilliSec();


  
		//Record key states
		if		( GetAsyncKeyState(VK_RIGHT) & 0x8000 )					setKeyState(X::XKeys::RIGHTARROW,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::RIGHTARROW,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_LEFT) & 0x8000 )					setKeyState(X::XKeys::LEFTARROW,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::LEFTARROW,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_UP) & 0x8000 )					setKeyState(X::XKeys::UPARROW,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::UPARROW,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_DOWN) & 0x8000 )					setKeyState(X::XKeys::DOWNARROW,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::DOWNARROW,X::XKeyState::UP);

		if		( GetAsyncKeyState('W') & 0x8000 )						setKeyState(X::XKeys::KEYW,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYW,X::XKeyState::UP);
		if		( GetAsyncKeyState('S') & 0x8000 )						setKeyState(X::XKeys::KEYS,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYS,X::XKeyState::UP);
		if		( GetAsyncKeyState('A') & 0x8000 )						setKeyState(X::XKeys::KEYA,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYA,X::XKeyState::UP);
		if		( GetAsyncKeyState('D') & 0x8000 )						setKeyState(X::XKeys::KEYD,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYD,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_SPACE) & 0x8000 )					setKeyState(X::XKeys::SPACE,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::SPACE,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_RETURN) & 0x8000 )				setKeyState(X::XKeys::ENTER,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::ENTER,X::XKeyState::UP);

		if		( GetAsyncKeyState(VK_F1) & 0x8000 )				setKeyState(X::XKeys::KEYF1,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF1,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F2) & 0x8000 )				setKeyState(X::XKeys::KEYF2,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF2,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F3) & 0x8000 )				setKeyState(X::XKeys::KEYF3,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF3,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F4) & 0x8000 )				setKeyState(X::XKeys::KEYF4,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF4,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F5) & 0x8000 )				setKeyState(X::XKeys::KEYF5,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF5,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F6) & 0x8000 )				setKeyState(X::XKeys::KEYF6,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF6,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F7) & 0x8000 )				setKeyState(X::XKeys::KEYF7,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF7,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F8) & 0x8000 )				setKeyState(X::XKeys::KEYF8,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF8,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F9) & 0x8000 )				setKeyState(X::XKeys::KEYF9,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF9,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F10) & 0x8000 )				setKeyState(X::XKeys::KEYF10,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF10,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F11) & 0x8000 )				setKeyState(X::XKeys::KEYF11,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF11,X::XKeyState::UP);
		if		( GetAsyncKeyState(VK_F12) & 0x8000 )				setKeyState(X::XKeys::KEYF12,X::XKeyState::DOWN);
		else														setKeyState(X::XKeys::KEYF12,X::XKeyState::UP);

		if		( GetAsyncKeyState('Z') & 0x8000 )						setKeyState(X::XKeys::KEYZ,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYZ,X::XKeyState::UP);
		if		( GetAsyncKeyState('X') & 0x8000 )						setKeyState(X::XKeys::KEYX,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYX,X::XKeyState::UP);
		if		( GetAsyncKeyState('C') & 0x8000 )						setKeyState(X::XKeys::KEYC,X::XKeyState::DOWN);
		else															setKeyState(X::XKeys::KEYC,X::XKeyState::UP);
	 





 
	t_DeltaTime =(double)(eTimer->getElapsedTimeInMilliSec()-lastTime);		 
	lastTime = eTimer->getElapsedTimeInMilliSec();
		 


	
		//fps readout
		if(GetTickCount()-lastFPSTick>=1000)
		{
			lastFPSTick=GetTickCount();		
			fps=lastFPS;			
			lastFPS=0;
		}
		else lastFPS++;



		//mechanics update per delta	
		if(ePhysics->isInitialized()==true)											ePhysics->Tick();
		if(eActorsManager->getActorsCount()>0)										eActorsManager->Tick();		
		if(eLightsManager->getLightsCount()>0)										eLightsManager->processLights();
		eCamera->Update();

 
 
		//Gui update
		if(guiLoaded==true)
		{
			eGui->Update();
		}
		 
		eEffectsManager->Tick();

 		return true;
	}
	float Engine::getCutSceneTimer()
	{
		return eTimer->getElapsedTime();
	}
	 
	/*
	*		render()
	*		arguments - none
	*		synopsis  - will render all in the batch	
	*/
	GLvoid Engine::render()
	{
	 
	 
		X::Shader *pActiveShader=NULL;


	  
		Viewport(screenSize);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		  
  


	

		 
 		if(eBsp->mapLoaded())
		{
			int a_Count=0;
		
			
		
			 
			Viewport(CVector2(512,512));
			eFBOManager->Bind("SHADOWMAPPING","",-1);	
			{


				glEnable(GL_POLYGON_OFFSET_FILL);			
				//Enable the internal ShadowPass shader
				pActiveShader=eShadersManager->getShader("internalShadowPass");
				if(pActiveShader==NULL)Quit();
				pActiveShader->TurnOn();
				
				int l_Count=eLightsManager->getLightsCount();
				if(l_Count>LIGHTS_MAX_DYNAMIC)l_Count=LIGHTS_MAX_DYNAMIC;


				LightData *l_Light=NULL;
				int LightsCt;
				for(LightsCt=0;LightsCt<l_Count;LightsCt++)
				{
					
									l_Light=eLightsManager->getLight(LightsCt);

									if(l_Light->castShadows==false )continue;
 
									//CUBE MAP (OMNI) requires 6 passes for each side
									for(int a=0;a<6;a++)
									 {

						 				eFBOManager->AttachTexture("SHADOWMAPPING",GL_DEPTH_ATTACHMENT,eLightsManager->tDepthCubeMap[LightsCt],GL_TEXTURE_CUBE_MAP_POSITIVE_X+a);
										glClear(GL_DEPTH_BUFFER_BIT);											
										
										//Calculate the frustum for this light view
										eFrustum->CalculateFrustum(eCamera->getProjectionMatrix(),eCamera->getViewMatrix());			 		
							 
										//Pass the light MVP 						
										pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_LightModelViewProjection"),l_Light->m_LightModelViewProjection[a]	); 
										
										//Pass the level Model transformation matrix
										pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_Model"),eBsp->m_Model); 
										eBsp->RenderLevel(true); 		 
	 								 
										//Manual actors render method to pass the matrix into the internalShadowPass shader
										int it;
										if(eActorsManager->getActorsCount()>0)
										{
											eStateManager->Set(CULLSIDE, CULLSIDE_BACK);				
											for(it=0;it<eActorsManager->getActorsCount();it++)
											{
												if(eActorsManager->getActor(it)->GetEntity()->castshadows==true)
												{
 															pActiveShader->SetMatrix4x4(pActiveShader->getUniform("m_Model"),eActorsManager->getActor(it)->m_Model); 
									 						eActorsManager->getActor(it)->m_pModel->renderModel(true);
												}
						 
											}
											eStateManager->Set(CULLSIDE, CULLSIDE_FRONT);
										}

										 

									 }

 					
				 				glActiveTexture(GL_TEXTURE0+LightsCt);
				 				glBindTexture(GL_TEXTURE_CUBE_MAP,eLightsManager->tDepthCubeMap[LightsCt]);

							}//for LightsCt
				
			}
			
			glDisable(GL_POLYGON_OFFSET_FILL);
			eFBOManager->Unbind("SHADOWMAPPING","");	 
			pActiveShader->TurnOff();

			 

			//FINAL RENDER
			Viewport(screenSize);	
			eFBOManager->Bind("FRAME","FRAME",-1);								//Bind the Frame/Render buffs
			//eFBOManager->AttachRenderbuffer("FRAME",GL_DEPTH_ATTACHMENT);	//Attach depth buffer, we need this in frame rendering
			glDrawBuffers(2, b_renderBuffers);								//Render to Two color buffers at once
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear color/depth
		  
			eFrustum->CalculateFrustum(eCamera->getProjectionMatrix(),eCamera->getViewMatrix());			 			
			//eLightsManager->calculateLighting(eCamera->Position(),eCamera,eFrustum);  -> NEEDS REPAIR TO SORT THE LIGHTS <-

			//Game->StateManager()->Set(X::POLYGONMODE,X::POLYGONMODE_LINE);

			//Render level with materials
			eBsp->RenderLevel(false);	

			//Render actors with materials			
			eActorsManager->Render(false);
	
			//Game->StateManager()->Set(X::POLYGONMODE,X::POLYGONMODE_FILL);
		 



		if(Physics()->getPhysicsDebugDrawing()==true)
		{
				if(Physics()->isInitialized()==true)
				{
					glMatrixMode(GL_PROJECTION);
					glLoadMatrixf(eCamera->getProjectionMatrix());	
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(eCamera->getViewMatrix());			 	
					eStateManager->Set(X::ZBUFFER,X::DISABLED);
						 LightsManager()->renderLightSources();
						 Physics()->getDynamic()->debugDrawWorld();
					eStateManager->Set(X::ZBUFFER,X::ENABLED);
					glMatrixMode(GL_PROJECTION);
				}
		}
	
		eFBOManager->Unbind("FRAME","FRAME");		
			 
	} 

	 

	


 
		 
 			//~~ PREPARE FOR ORTHO RENDERING FINALFRAME + GUI ~~//
			renderOrtho(screenSize);
			eStateManager->Set(BFCULLING,       DISABLED);			
			glActiveTexture(XTEXTURE0);
			Game->StateManager()->Set(TEXTURING,		ENABLED);

		 

			//~~ EFFECTS MANAGER~~//
			eEffectsManager->renderEffects();
		 
			

			//~~ PREPARE FRAME TEXTRE AND DUMP ONTO THE SCREEN ~~//		
			glActiveTexture(XTEXTURE0);
			glClientActiveTextureARB(XTEXTURE0);		
			 
			// DUMP FRAME ONTO THE SCREEN //
			glBindTexture(GL_TEXTURE_2D,t_FrameTexture);
			glCallList(l_FrameRenderList);
			
			

 			// RENDER GUI //		 

			/*
			if(guiLoaded==true)
			{
					eStateManager->Set(X::ZBUFFER,X::DISABLED);
					eStateManager->Set(X::BLENDING,X::ENABLED);
					eGui->Render();				
					eStateManager->Set(X::BLENDING,X::DISABLED);			
					eStateManager->Set(X::ZBUFFER,X::ENABLED);
				
 			}    
			*/
			



			//CLEAR THE RENDER STATES //
			eStateManager->Set(BFCULLING,       ENABLED);
			Game->StateManager()->Set(TEXTURING,		DISABLED);
			renderPerspective();

		
			 

		/**/
		SwapBuffers(hdcMain);
	}

	GLuint Engine::getSSAOTexture()
	{
		return t_FrameSSAO;
	}
	GLuint Engine::getFrameTexture()
	{
		return t_FrameTexture;
	}

	/*
	*		run()
	*		arguments - none
	*		synopsis  - runs engine ticks and render
	*/
	GLvoid Engine::run()
	{

	
		//Perform per-tick processing
		tick();
		 
		//render
	 	render();
		 
		
	}





	 
	 
 




	/*
	*		loadShaders()
	*		arguments - none
	*		synopsis  - will perform automated load of the shaders
	*/
	bool Engine::loadShaders()
	{
		Shader *shader=NULL;

		eLog->write("<%s:%i %s> Started",__FILE__,__LINE__,__FUNCTION__);




		//Load and setup shader
		if(eShadersManager->loadShader("internalShadowPass","shadowPass","shadowPass")==false)
		{
			
			return false;
		}
		else
		{
			shader=eShadersManager->getShader("internalShadowPass");
			if(shader==NULL)return false;
			shader->TurnOn();
			shader->TurnOff();

		}
		 

		//Load and setup shader
		if(eShadersManager->loadShader("internalSSAO","ssaoPass","ssaoPass")==false)
		{
			
			return false;
		}
		else
		{
			shader=eShadersManager->getShader("internalSSAO");
			if(shader==NULL)return false;
			shader->TurnOn();
			shader->SetInt(shader->getUniform("t_FrameDepthTexture"),2);
			shader->TurnOff();

		}
		 

		return true;
	}



	/*
	*	reloadGUI()
	*	arguments - 
	*	synopsis	- reloads actually loaded GUI
	*/
	bool Engine::reloadGUI()
	{
		eLog->write("<%s:%i %s> Reloading GUI",__FILE__,__LINE__,__FUNCTION__);
		return loadGUI(guiScheme,guiSchemeName,guiTheme,guiThemeName,guiEvents,guiEventsCt);
	}

	/*
	*		loadGUI()
	*		arguments - scheme	scheme_name	theme	theme_name	GUI_lastevent	eventsCt
	*		synopsis  - performs automated GUI scheme render
	*					and flags last gui for 'reload' possibilities in development enviroment
	*/
	bool Engine::loadGUI( char *scheme, char *scheme_name, char *theme, char *theme_name,Gui::Event*	GUI_lastevent,  int eventsCt)
	{ 
 
		eGui->Cleanup();
		guiLoaded=false;


	 

 
		eLog->write("<%s:%i %s> Injecting GUI loader",__FILE__,__LINE__,__FUNCTION__);
		eLog->write("<%s:%i %s> Scheme: %s",__FILE__,__LINE__,__FUNCTION__,scheme);
		eLog->write("<%s:%i %s> Scheme_name: %s",__FILE__,__LINE__,__FUNCTION__,scheme_name);
		eLog->write("<%s:%i %s> Theme: %s",__FILE__,__LINE__,__FUNCTION__,theme);
		eLog->write("<%s:%i %s> Theme_name: %s",__FILE__,__LINE__,__FUNCTION__,theme_name);

		if (eGui->LoadTheme(	theme,theme_name)==false)
		{
			eLog->write("<%s:%i %s> Cannot load GUI theme",__FILE__,__LINE__,__FUNCTION__);
		
			return false;
		}
  
		eLog->write("<%s:%i %s> Theme loaded",__FILE__,__LINE__,__FUNCTION__);

		if(eGui->LoadScheme(	scheme,	scheme_name,GUI_lastevent,eventsCt)==false)		
		{

			eLog->write("<%s:%i %s> Cannot load GUI scheme",__FILE__,__LINE__,__FUNCTION__);
			
			return false;
		};
	 	eLog->write("<%s:%i %s> Scheme loaded",__FILE__,__LINE__,__FUNCTION__);
	 
	 eGui->Update();

	eLog->write("<%s:%i %s> GUI loaded OK.",__FILE__,__LINE__,__FUNCTION__);
	

 	sprintf(guiScheme,"%s",scheme);
	sprintf(guiTheme,"%s",theme);
	sprintf(guiSchemeName,"%s",scheme_name);
	sprintf(guiThemeName,"%s",theme_name);
	guiEvents=GUI_lastevent;
	guiEventsCt=eventsCt;


	guiLoaded=true;
	return true; 
	 
	}




	
	/*
	*	setKeyState(XKeys k_Key,XKeyState k_KeyState)
	*	arguments	- key,state
	*	synopsis	- PRIVATE, sets key state
	*/
	void Engine::setKeyState(XKeys k_Key,XKeyState k_KeyState)
	{
		s_KeyStates[k_Key]=k_KeyState;
	}




	
	/*
	*	getKeyState(XKeys k_Key)
	*	arguments	- key
	*	synopsis	- returns 'key' UP/DOWN status
	*/
	XKeyState Engine::getKeyState(XKeys k_Key)
	{
		return (XKeyState) s_KeyStates[k_Key];
	}



	int   Engine::getFPS()
	{
		return fps;

	}
	double Engine::getDelta()
	{
		return t_DeltaTime;
	}
	/*
	*	getGUI()
	*	arguments - 
	*	synopsis - returns pointer to engine gui
	*/
	Gui::IGui *Engine::XGui()
	{
		if(guiLoaded)		return eGui;
		return NULL;
	}



	HWND Engine::getHWND()
	{
		return hwndMain;
	} 

	 
 
GLvoid Engine::renderOrtho(CVector2 screenSize)
{
	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0, screenSize.x , screenSize.y, 0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();													// Push The Matrix
	glLoadIdentity();											// Reset The Matrix

}

GLvoid Engine::renderPerspective()
{
	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix
}
GLuint Engine::getFrameDepthTexture()
{
	return t_FrameTextureDepth;
}
GLuint Engine::getFrameTextureOriginal()
{
	return t_FrameTextureOriginal;
}

GLuint Engine::getFrameRenderList()
{
	return l_FrameRenderList;
}
CVector2 Engine::getScreenSize()
{
	return screenSize;
}
void Engine::AddRef()
{
        m_refCount++;
}

void Engine::Release()
{
        m_refCount--;
}
 }
