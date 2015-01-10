#ifndef _SCOREBOARD_HEADER_
#define _SCOREBOARD_HEADER_

#include "modules/base.h"
#include "util/transitions.h"
#include "panels/panel.h"
#include "room/room.h"

class ScoreboardM : public GModule {
public:
	virtual void Initialize();
	virtual void Clean();
	virtual void Draw();
	virtual void Think();
	virtual void BGThink();
	virtual void Event(SDL_Event &kevent);

	void SetTransition(Util::SteelTransition* nTrans);
	void SetScores(RoomScoreData nScores);

	ScoreboardM();
	~ScoreboardM();
protected:
	Util::SteelTransition* openTrans;
	RoomScoreData scoreData;
	int StartTime;

	char* cNpcs;
	char* cDamage;
	char* cMoney;
	DFrame* mainPanel;
private:

};



#endif