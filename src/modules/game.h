#ifndef _GAME_HEADER_
#define _GAME_HEADER_

#include "modules/base.h"
#include "room/room.h"
#include "util/transitions.h"

class GameModu : public GModule {
public:
	virtual void Initialize();
	virtual void Clean();
	virtual void Draw();
	virtual void Think();
	virtual void BGThink();
	virtual void Event(SDL_Event &kevent);

	void ResetPan();
	void NewStage(RoomStage newStage);
	GameModu();
	~GameModu();
	Pos2D GetPanPos();
protected:
	int centerX, centerY;
	int panX, panY;
	int oClickX, oClickY;
	int oPanX, oPanY;
	int viewPX, viewPY;
	bool panning;
	TRoom* activeRoom;
	char* mouseEntity;

	bool transitioning;
	float tranStage;
	unsigned int tranStart;

	Util::SteelTransition* closeTrans;
};

#endif