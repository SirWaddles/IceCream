#ifndef _PATHFIND_HEADER_
#define _PATHFIND_HEADER_

#include "room/room.h"
#include <list>
#include <deque>

namespace PFind{
	struct CellCo{
		Pos2D pos;
		int counter;
		CellCo(int x, int y, int count) : pos(x, y){
			counter=count;
		}
		CellCo(Pos2D cPos, int count) : pos(cPos.x, cPos.y){
			counter=count;
		}
		CellCo() : pos(0,0){
			counter=0;
		}
	};

	struct PathData{
		std::list<CellCo> mainPath;
		Pos2D pSize;
		Pos2D pathStart;
		Pos2D pathEnd;
		TRoom* pathRoom;
	};

	//bool FindPath(bool stopAtStart=false);
	bool FindPathS(bool stopAtStart, Pos2D size, PathData* pData);
	void SetStart(Pos2D start, PathData* pData);
	void SetEnd(Pos2D end, PathData* pData);
	void SetRoom(TRoom* room, PathData* pData);
	//std::list<CellCo> GetPath();
	//std::deque<Pos2D> GetPathD();
	std::deque<Pos2D> GetPathDS(Pos2D size, PathData* pData);
}

#endif