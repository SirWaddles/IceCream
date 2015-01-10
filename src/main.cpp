#include "graphics/window.h"
//#include "modules/base.h"
#include "modules/main_menu.h"

#include "util/leveldata.h"
#include "util/timem.h"

#define DELTA_INT 30

unsigned int nextTick=0;

unsigned int MsLeft(){
	unsigned int now;
	now=SDL_GetTicks();
	if(nextTick<=now){
		nextTick=now+DELTA_INT;
		return 0;
	}
	return (nextTick-now);
}

bool quitGame=false;

void ForceQuit(){
	quitGame=true;
}

int main(int argc, char *argv[]){
	GraphicsM::StartGraphics();
	GraphicsM::CreateMainWindow();
	SDL_Event kevent;
	GModule *gmod=new MainMenu();
	Util::LoadAllLevels();
	Util::LoadAllCampaigns();
	GMods::HookModule(gmod);
	GMods::SetFocus(gmod);
	while(!quitGame){
		Util::UpdateFrameTime();
		//printf("FrameTime: %f\n", 1.0/Util::FrameTime());
		GMods::DoThink();
		GMods::DoDraw();
		GMods::DoBGThink();
		while(SDL_PollEvent(&kevent)){
			if(kevent.type==SDL_QUIT){
				quitGame=true;
				break;
			}
			if(kevent.type==SDL_VIDEORESIZE){
				GraphicsM::ResizeWindow(kevent.resize.w, kevent.resize.h);
			}
			GMods::RunEvent(kevent);
		}
		//SDL_Delay(MsLeft());
	}
	GMods::RemoveHook(gmod);
	GraphicsM::EndGraphics();
	return 0;
}