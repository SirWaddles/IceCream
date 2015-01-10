#ifndef _MAINMENU_HEADER_
#define _MAINMENU_HEADER_

#include "modules/base.h"

class MainMenu : public GModule {
public:
	virtual void Initialize();
	virtual void Clean();
	virtual void Draw();
	virtual void Think();
	virtual void BGThink();
	virtual void Event(SDL_Event &kevent);
	virtual void ChangeModule(int nIndex);
	MainMenu();
	~MainMenu();

	void StartTransition();
	void (*transAct)();
private:
	bool transitioning;
	unsigned int tranStart;
	
	
};


#endif