#ifndef __XENGINE_H
#define __XENGINE_H

 
namespace X
{

	//Shaders types
	enum EShaders
	{

		XSHADER_FULL,
		XSHADER_SIMPLE,
		XSHADER_XYN,
		XSHADER_GUI,
		XSHADER_TEST,
		XSHADER_DEPTH,
		XSHADERS_TOTAL

	};
	//Render types
	enum ERenderType
	{
	 XRENDER_WIREFRAME,
	 XRENDER_FILL,
	 XRENDER_TYPES_TOTAL
	};	
	//KeyStates
	enum XKeyState
	{
		UP=0,
		DOWN,
		KEYSTATES_TOTAL
	};
	//Keys
	enum XKeys
	{
		RIGHTARROW=0,
		LEFTARROW,
		UPARROW,
		DOWNARROW,
		KEYW,
		KEYS,
		KEYA,
		KEYD,
		SPACE,
		ENTER,
		KEYF1,
		KEYF2,
		KEYF3,
		KEYF4,
		KEYF5,
		KEYF6,
		KEYF7,
		KEYF8,
		KEYF9,
		KEYF10,
		KEYF11,
		KEYF12,
			
		KEYZ,
		KEYX,
		KEYC,
		KEYS_TOTAL
	};


	class Engine
	{


		public:
			/*
			METHODS
			*/
			Engine();
			void Init(int screenWidth, int screenHeight, char *title,WNDPROC procWnd,HICON icon);
			GLvoid renderCallback( void (*fct)());
			GLvoid run();
			void Quit();
			GLuint getFrameTexture();
			GLuint getFrameTextureOriginal();
			GLuint getFrameDepthTexture();
			GLuint getSSAOTexture();
			CVector2 getScreenSize();
			GLvoid renderOrtho(CVector2 screenSize);
			GLvoid renderPerspective();
			GLvoid Viewport(CVector2 size);
			GLuint getFrameRenderList();
			float getCutSceneTimer();
			 


			//@@ GUI
			bool loadGUI( char *scheme, char *scheme_name, char *theme, char *theme_name,Gui::Event*	GUI_lastevent,  int eventsCt);
			bool reloadGUI();

			//@@ Will go to private later on
			bool loadShaders();
 			
 		 	//@@Destruct
			~Engine();
 
			/*
			*	gets - methods
			*/
			
			
			double				getDelta();	
			int					getFPS();
			HWND				getHWND();
			XKeyState			getKeyState(XKeys k_Key);
			X::CCamera			*Camera();
			X::CActorsManager	*ActorsManager();
			X::CLightsManager	*LightsManager();
			X::CPhysicsManager	*Physics();
			X::CLogManager		*Log();
			X::CTexturesManager	*TexturesManager();
			X::CStateManager	*StateManager();
			X::CFrustum			*Frustum();
			X::CBuffersManager	*FBOManager();
			Gui::IGui			*XGui();
			XBSP				*Level();		 
			X::CScriptsManager	*Scripting();
			XTimer::CTimer		*eTimer;
			X::CEffectsManager	*EffectsManager();
			X::CMaterialsManager *MaterialsManager();
			X::CShadersManager	*ShadersManager();
			X::CEntitiesManager *EntitiesManager();
			
			
			void AddRef();
			void Release();
 		 

		private:

			/*
			VARS
			*/

			//@@ Windows vars
	 		HWND hwndMain;
			HINSTANCE hInstance;
			WNDCLASS wndclass;
			HDC hdcMain;
			HGLRC hglrc1;
			CVector2 screenSize;
			HICON i_Application;
			int	  s_KeyStates[KEYS_TOTAL];

			//@@ Gui scheme/theme names for later "reload/setScheme use"
			char guiScheme[128];
			char guiTheme[128];
			char guiSchemeName[128];
			char guiThemeName[128];
			Gui::Event*	guiEvents;
			int guiEventsCt;
			
			
			//@@ Framerate vars
			double t_DeltaTime;			//Actual deltaTime since last frame
			float t_Elapsed;			//Time elapsed since start (in ms)
			int fps;					//Actual FPS measurements
		

	 	
			//@@ Loader flags
			bool mapLoaded;
			bool modelsLoaded;
			bool guiLoaded;
   
			
			/*
			OBJECTS
			*/
			X::CFrustum				*eFrustum;
			X::CBuffersManager		*eFBOManager;
			X::XBSP					*eBsp;
			Gui::IGui				*eGui; 
			X::CStateManager		*eStateManager;
 			X::CCamera				*eCamera;
			X::CActorsManager		*eActorsManager;
			X::CLightsManager		*eLightsManager;
			X::CPhysicsManager		*ePhysics;
			X::CLogManager			*eLog;
			X::CTexturesManager		*eTexturesManager;				
			X::CScriptsManager		*eScripting;
			X::CEffectsManager		*eEffectsManager;
			X::CMaterialsManager	*eMaterialsManager;
			X::CShadersManager		*eShadersManager;
			X::CEntitiesManager		*eEntitiesManager;

			/*
			METHODS
			*/
			void (*renderCallbackFunc)();
			bool tick();
			GLvoid render();	

			
			void setKeyState(XKeys k_Key,XKeyState k_KeyState);
			HWND createWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen,WNDPROC procWnd);
			bool initGL();
 

			GLenum	b_renderBuffers[2];			
			GLuint	t_FrameTexture;				//This one gets modified by xffects
			GLuint	t_FrameTextureOriginal;		//Not modificable
			GLuint	t_FrameSSAO;				//SSAO ready texture
			GLuint	t_FrameTextureDepth;		//Depth texture from the main frame
			GLuint	l_FrameRenderList;
			

			int m_refCount;
	};
 

 
}


#endif