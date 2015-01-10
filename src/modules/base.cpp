#include "modules/base.h"
#include "panels/panel.h"
#include "room/room.h"
#include "entities/entity.h"
//#include <vector>
#include <list>

using namespace std;

void GModule::Initialize(){

}

void GModule::Clean(){

}

void GModule::Draw(){
}

void GModule::Think(){

}

void GModule::BGThink(){

}

void GModule::Event(SDL_Event &kevent){

}

void GModule::ChangeModule(GModule* newModule){

}

GModule::GModule(){
	Initialize();
	mCode="base";
}

GModule::~GModule(){
	Clean();
}

namespace GMods {
	GModule* Focus;
	list<GModule*> Modules;

	void SetFocus(GModule* sFocus){
		Focus=sFocus;
		list<GModule*>::iterator it;
		for(it=Modules.begin(); it != Modules.end(); it++){
			(*it)->ChangeModule(sFocus);
		}
	}

	
	void HookModule(GModule *gmod){
		Modules.push_back(gmod);
	}

	GModule* GetModule(char* code){
		list<GModule*>::iterator it;
		for(it=Modules.begin(); it!=Modules.end(); it++){
			if(strcmp((*it)->mCode, code)==0){
				return (*it);
			}
		}
		return NULL;
	}

	void RemoveHook(GModule *gmod){
		if(gmod==Focus){
			Focus=*Modules.begin();
		}
		Modules.remove(gmod);
		delete gmod;
	}

	void RunEvent(SDL_Event &kevent){
		Focus->Event(kevent);
	}

	void DoDraw(){
		Focus->Draw();
		GraphicsM::WaitForFinish();
		SDL_GL_SwapBuffers();
	}

	void DoThink(){
		Focus->Think();
	}

	void DoBGThink(){
		list<GModule*>::iterator it;
		for(it=Modules.begin(); it != Modules.end(); it++){
			(*it)->BGThink();
		}
	}
}