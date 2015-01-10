#include "pathfind.h"

using namespace std;

namespace PFind{

	void SetStart(Pos2D start, PathData* pData){
		pData->pathStart=start;
	}

	void SetEnd(Pos2D end, PathData* pData){
		pData->pathEnd=end;
	}

	void SetRoom(TRoom* room, PathData* pData){
		pData->pathRoom=room;
	}

	bool CheckCell(CellCo* cell, list<CellCo>* mainList, TRoom* room){
		list<CellCo>::iterator it;
		for(it=mainList->begin(); it!=mainList->end(); it++){
			if(cell->pos==it->pos && it->counter<=cell->counter){
				return false;
				break;
			}
		}
		if(room->IsOccupied(cell->pos.x, cell->pos.y)){
			return false;
		}
		
		return true;
	}

	/*bool FindPath(bool stopAtStart){
		mainPath.clear();
		mainPath.push_back(CellCo(pathEnd, 0));
		list<CellCo>::iterator it;
		bool success=false;
		for(it=mainPath.begin();it!=mainPath.end();it++){
			//if(it->pos.x==start.x&&it->pos.y==start.y){
			if(it->pos==pathStart&&stopAtStart){
				//Success
				success=true;
				break;
			}
			//Up
			CellCo up=CellCo(it->pos.x, it->pos.y+1, it->counter+1);
			if(up.pos==pathStart&&stopAtStart){
				mainPath.push_back(up);
				success=true;
				break;
			}
			if(CheckCell(&up, &mainPath, pathRoom)){
				mainPath.push_back(up);
				//printf("Adding X: %d  Y: %d\n", up.pos.x, up.pos.y);
			}
			//Down
			CellCo down=CellCo(it->pos.x, it->pos.y-1, it->counter+1);
			if(down.pos==pathStart&&stopAtStart){
				mainPath.push_back(down);
				success=true;
				break;
			}
			if(CheckCell(&down, &mainPath, pathRoom)){
				mainPath.push_back(down);
				//printf("Adding X: %d  Y: %d\n", down.pos.x, down.pos.y);
			}
			//Left
			CellCo left=CellCo(it->pos.x-1, it->pos.y, it->counter+1);
			if(left.pos==pathStart&&stopAtStart){
				mainPath.push_back(left);
				success=true;
				break;
			}
			if(CheckCell(&left, &mainPath, pathRoom)){
				mainPath.push_back(left);
				//printf("Adding X: %d  Y: %d\n", left.pos.x, left.pos.y);
			}
			//Right
			CellCo right=CellCo(it->pos.x+1, it->pos.y, it->counter+1);
			if(right.pos==pathStart&&stopAtStart){
				mainPath.push_back(right);
				success=true;
				break;
			}
			if(CheckCell(&right, &mainPath, pathRoom)){
				mainPath.push_back(right);
				//printf("Adding X: %d  Y: %d\n", right.pos.x, right.pos.y);
			}
		}
		if(!success){
			printf("Path not found.");
			return false;
		}
		printf("Wat: %d\n", mainPath.size());
		return true;

	}*/

	bool IsInRange(Pos2D cellP, Pos2D cellP2, Pos2D size){
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				if(cellP==Pos2D(x+cellP2.x, y+cellP2.y)){
					return true;
				}
			}
		}
		return false;
	}

	bool FindPathS(bool stopAtStart, Pos2D size, PathData* pData){
		//printf("FindPath: %d\n", SDL_GetTicks());
		pData->mainPath.clear();
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				pData->mainPath.push_back(CellCo(Pos2D(pData->pathEnd.x+x, pData->pathEnd.y+y), 0));
			}
		}
		list<CellCo>::iterator it;
		bool success=false;
		for(it=pData->mainPath.begin();it!=pData->mainPath.end();it++){
			//if(it->pos.x==start.x&&it->pos.y==start.y){
			if(it->pos==pData->pathStart&&stopAtStart){
				//Success
				success=true;
				break;
			}
			//Up
			CellCo up=CellCo(it->pos.x, it->pos.y+1, it->counter+1);
			if(up.pos==pData->pathStart&&stopAtStart){
				pData->mainPath.push_back(up);
				success=true;
				break;
			}
			if(CheckCell(&up, &pData->mainPath, pData->pathRoom)){
				pData->mainPath.push_back(up);
				//printf("Adding X: %d  Y: %d\n", up.pos.x, up.pos.y);
			}
			//Down
			CellCo down=CellCo(it->pos.x, it->pos.y-1, it->counter+1);
			if(down.pos==pData->pathStart&&stopAtStart){
				pData->mainPath.push_back(down);
				success=true;
				break;
			}
			if(CheckCell(&down, &pData->mainPath, pData->pathRoom)){
				pData->mainPath.push_back(down);
				//printf("Adding X: %d  Y: %d\n", down.pos.x, down.pos.y);
			}
			//Left
			CellCo left=CellCo(it->pos.x-1, it->pos.y, it->counter+1);
			if(left.pos==pData->pathStart&&stopAtStart){
				pData->mainPath.push_back(left);
				success=true;
				break;
			}
			if(CheckCell(&left, &pData->mainPath, pData->pathRoom)){
				pData->mainPath.push_back(left);
				//printf("Adding X: %d  Y: %d\n", left.pos.x, left.pos.y);
			}
			//Right
			CellCo right=CellCo(it->pos.x+1, it->pos.y, it->counter+1);
			if(right.pos==pData->pathStart&&stopAtStart){
				pData->mainPath.push_back(right);
				success=true;
				break;
			}
			if(CheckCell(&right, &pData->mainPath, pData->pathRoom)){
				pData->mainPath.push_back(right);
				//printf("Adding X: %d  Y: %d\n", right.pos.x, right.pos.y);
			}
		}
		if(!success){
			printf("Path not found.");
			printf("Wat: %d\n", pData->mainPath.size());
			return false;
		}
		printf("Wat: %d\n", pData->mainPath.size());
		return true;

	}

	/*list<CellCo> GetPath(){
		list<CellCo> finalPath;
		list<CellCo>::iterator it;
		CellCo activeCell=CellCo(pathStart, 0);
		while(!(activeCell.pos.x==pathEnd.x&&activeCell.pos.y==pathEnd.y)){
			CellCo nextCell=CellCo(0, 0, 1000);
			for(it=mainPath.begin();it!=mainPath.end();it++){
				
				//Up
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y+1&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Down
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y-1&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Left
				if(it->pos.x==activeCell.pos.x-1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Right
				if(it->pos.x==activeCell.pos.x+1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
			}
			if(nextCell.counter>=1000){
				break;
			}
			activeCell=nextCell;
			finalPath.push_back(nextCell);
		}
		return finalPath;
	}

	deque<Pos2D> GetPathD(){
		deque<Pos2D> finalPath;
		list<CellCo>::iterator it;
		CellCo activeCell=CellCo(pathStart, 0);
		while(!(activeCell.pos.x==pathEnd.x&&activeCell.pos.y==pathEnd.y)){
			CellCo nextCell=CellCo(0, 0, 1000);
			for(it=mainPath.begin();it!=mainPath.end();it++){
				
				//Up
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y+1&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Down
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y-1&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Left
				if(it->pos.x==activeCell.pos.x-1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
				//Right
				if(it->pos.x==activeCell.pos.x+1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
				}
			}
			if(nextCell.counter>=1000){
				break;
			}
			activeCell=nextCell;
			finalPath.push_back(nextCell.pos);
		}
		return finalPath;
	}*/

	deque<Pos2D> GetPathDS(Pos2D size, PathData *pData){
		deque<Pos2D> finalPath;
		list<CellCo>::iterator it;
		CellCo activeCell=CellCo(pData->pathStart, 0);
		while(!(IsInRange(activeCell.pos, pData->pathEnd, size))){
			CellCo nextCell=CellCo(0, 0, 1000);
			for(it=pData->mainPath.begin();it!=pData->mainPath.end();it++){
				//Up
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y+1&&it->counter<nextCell.counter){
					nextCell=(*it);
					break;
				}
				//Down
				if(it->pos.x==activeCell.pos.x&&it->pos.y==activeCell.pos.y-1&&it->counter<nextCell.counter){
					nextCell=(*it);
					break;
				}
				//Left
				if(it->pos.x==activeCell.pos.x-1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
					break;
				}
				//Right
				if(it->pos.x==activeCell.pos.x+1&&it->pos.y==activeCell.pos.y&&it->counter<nextCell.counter){
					nextCell=(*it);
					break;
				}
			}
			if(nextCell.counter>=1000){
				break;
			}
			activeCell=nextCell;
			finalPath.push_back(nextCell.pos);
		}
		//printf("MakePath: %d\n", SDL_GetTicks());
		return finalPath;
	}

}