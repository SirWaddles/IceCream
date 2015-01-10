#ifndef _BASE_HEADER_
#define _BASE_HEADER_

#include "SDL.h"
#include "graphics\window.h"

class GModule {
public:
	GModule();
	~GModule();
	virtual void Initialize();
	virtual void Clean();
	virtual void Draw();
	virtual void Think();
	virtual void BGThink();
	virtual void Event(SDL_Event &kevent);
	virtual void ChangeModule(GModule* newModule);
	char* mCode;
private:
	

};

namespace GMods {
	void HookModule(GModule *gmod);
	void RemoveHook(GModule *gmod);
	void RunEvent(SDL_Event &kevent);
	void SetFocus(GModule* sFocus);
	void DoDraw();
	void DoThink();
	void DoBGThink();
	GModule* GetModule(char* code);
}

#endif