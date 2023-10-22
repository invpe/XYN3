#define GAME_VERSION 10
#include "resource.h"
#include "../xengine/xmain.h"

#ifdef _DEBUG
#pragma comment(lib,"../xengined.lib")
#else
#pragma comment(lib,"../xengine.lib")
#endif

X::Engine *Game;


#include "gamegui.h"
HANDLE hGameLogicThread ;
DWORD  tNetworkThread ;
DWORD WINAPI _threadWrapper(void *parm);
 

 
 

LRESULT CALLBACK localWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	
	  
	switch(Message)
	{
		case WM_CREATE:
			
		break;




		case WM_KEYUP:
		{
 
			switch(wParam)
			{
			}

		}
		break;

 
	
		case WM_KEYDOWN:					 
		{
 
			switch(wParam)
			{
			
			 

				case VK_F1:
					//Game->reloadGUI();
					// Game->ActorsManager()->getActor("Awesome")->RotateTo("s");
				break;
				case VK_F2:
					 
				break;
				case VK_F3:
					{
						//CVector3 ap=Game->EntitiesManager()->getEntityClass("Camera_waypoint")->origin;
						//CVector3 ap=Game->ActorsManager()->getActor("Awesome")->GetPosition();
						//Game->ActorsManager()->getActor("Fuse1")->Activate();
						//Game->ActorsManager()->getActor("Fuse1")->ClearVelocities();
						//Game->ActorsManager()->getActor("Fuse1")->SetPosition(CVector3(ap.x,ap.y+60,ap.z));
						//Game->ActorsManager()->DeleteActor("Lamp");
					 

					}
				break;
				case VK_F6:
					{
						if(Game->Physics()->getPhysicsDebugDrawing()==false)Game->Physics()->setPhysicsDebugDrawing(true);
						else Game->Physics()->setPhysicsDebugDrawing(false);
					}
				break;
				 
		 
 				case VK_F5: 
					
					Game->MaterialsManager()->reloadMaterials();
					
						 
				break;
  
				case VK_F4:
					{
						//Game->ShadersManager()->reloadShaders();
						
					}
				break;
				

				case VK_ESCAPE:
					TerminateThread(hGameLogicThread,0);
					Game->Log()->write("User calling QUIT");
					Game->~Engine();
					exit(0);
				break;

			}

		}
		break;

	
	} 

return DefWindowProc(hwnd, Message, wParam, lParam);

 

}

DWORD WINAPI _threadWrapper(void *sleeptime)
{

while(1)
{
Game->Scripting()->Run("GameLoop");
Sleep(50);
}

return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{ 

	MSG Msg;			
	
	Game=new X::Engine;
	Game->Init(1024,768,"Awesome Guy",localWndProc,LoadIcon(hInstance,"IDI_ICON1"));
	Game->loadGUI("data/gui/GuiS.xml","Root","data/gui/GuiT.xml","Glass",GUI_events,1);
 
	Game->Scripting()->Init();
	if(Game->Scripting()->LoadScript("data/scripts/level0.xs","Level")==false)
	{
	 
	}
	Game->Scripting()->Run("Init");
 	 		 
 
	int milliseconds=50;
	hGameLogicThread=CreateThread( NULL, 50,  _threadWrapper, reinterpret_cast<void*>(&milliseconds),  0, &tNetworkThread );
 
	while(1)									 
	{			 	

					if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) 
					{ 
						if(Msg.message == WM_QUIT)	break;
						TranslateMessage(&Msg);						 
						DispatchMessage(&Msg);						 
					}
					else				
					{							 
					 
		 				     char txt[256];sprintf(txt,"FPS: %i, DELTA: %1.3fs, LIGHTS: %i ACTORS: %i | BSP visFaces: %i, actCluster: %i | EffectsQ: %i | CutsceneTimer: %1.1f",Game->getFPS(),Game->getDelta()/1000.0f,Game->LightsManager()->getLightsCount(),  Game->ActorsManager()->getActorsCount(),Game->Level()->getVisibleFacesCount(),Game->Level()->getCluster(Game->Camera()->Position()),Game->EffectsManager()->getEffectsQueued(),Game->eTimer->getElapsedTime());
							 Game->XGui()->SetText(txt,Game->XGui()->GetControlByName("t_Status"));	
							 
 
						Game->run();
					 
					 } 
					 
	}



	delete(Game);
	return(Msg.wParam);	


}
