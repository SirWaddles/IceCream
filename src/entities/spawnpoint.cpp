#include "entities/manager.h"
#include "entities/npcs.h"
#include "graphics/window.h"
#include "room/room.h"

using namespace Ents;
using namespace std;

SpriteSheet* spawnPointTex=NULL;

const bool ESpawnPoint::Purchasable=false;
const int ESpawnPoint::Price=0;
const Pos2D ESpawnPoint::pSize=Pos2D(1,1);

static EManEntry<ESpawnPoint> ent_npc_spawn("ent_npc_spawn");

void ESpawnPoint::Draw(){
	int pX, pY;
	pX=xCell*TILE_SIZE_X;
	pY=yCell*TILE_SIZE_Y;
	GraphicsM::DrawSprite(pX, pY, spriteNo, spawnPointTex);
}

void ESpawnPoint::Event(SDL_Event &kevent){
	if(kevent.type==SDL_USEREVENT){
		printf("Running Event\n");
		if(kevent.user.code==2&&pathFindThread){
			MutexPData* pData=static_cast<MutexPData*>(kevent.user.data1);
			PathFound(pData);
		}
	}
}

void ESpawnPoint::Think(){
	if(parentRoom->GetStage()==STAGE_GAME){
		if(SDL_GetTicks()>lastSpawn+(spawnTime*1000)){
			CreateNextNPC();
			lastSpawn=SDL_GetTicks();
		}
	}
}

ESpawnPoint::ESpawnPoint(){
	yRel=0;
	xRel=0;
	spriteNo=5;
	spawnDir=SPAWN_SELF;
	canFind=false;
	Moveable=false;
	if(!spawnPointTex){
		spawnPointTex=new SpriteSheet("contents/spawner.png", 64, 45);
	}
	DrawOrder=0;
	targetEnt=NULL;
	pathFindThread=NULL;

	desHq=NULL;
	newHq=NULL;
}

bool ESpawnPoint::IsOccupy(int x, int y){
	if(xCell==x&&yCell==y){
		return true;
	}
	return false;
}

Sprite ESpawnPoint::GetSprite(){
	Sprite spr;
	return spr;
}

void ESpawnPoint::SetPosition(int x, int y){
	xCell=x;
	yCell=y;
	if(y==0){
		spriteNo=4;
		spawnDir=SPAWN_UP;
		yRel=TILE_SIZE_Y;
	}
	if(y==parentRoom->GetSizeY()-1){
		spriteNo=3;
		spawnDir=SPAWN_DOWN;
		yRel=-TILE_SIZE_Y;
	}
	if(x==0){
		spriteNo=1;
		spawnDir=SPAWN_RIGHT;
		xRel=TILE_SIZE_X;
	}
	if(x==parentRoom->GetSizeX()-1){
		spriteNo=2;
		spawnDir=SPAWN_LEFT;
		xRel=-TILE_SIZE_X;
	}
}

void ESpawnPoint::StringParam(string str){
	spawnQueue.clear();
	char* toTok=new char[str.length()+2];
	strcpy(toTok, str.c_str());
	char* buf;
	buf=strtok(toTok, ",");
	spawnTime=atoi(buf);
	while(buf!=NULL){
		buf=strtok(NULL, ",");
		if(!buf){
			break;
		}
		SpawnQueueData eData;
		eData.eName=buf;
		buf=strtok(NULL, ",");
		if(!buf){
			break;
		}
		eData.quantity=atoi(buf);
		spawnQueue.push_back(eData);
	}

	delete[] toTok;
}

bool ESpawnPoint::StateChange(RoomStage newStage){
	if(newStage==STAGE_GAME){
		lastSpawn=SDL_GetTicks();
		CreatePath();
	}
	return canFind;
}

void ESpawnPoint::CreateNextNPC(){
	if(spawnQueue.size()<=0){
		return;
	}
	SpawnQueueData &spawn=spawnQueue.front();
	if(spawn.eName.length()>0){
		//printf("Spawned: %s at %d, %d\n", spawn.eName.c_str(), xCell, yCell);
		//printf("Relative: %f, %f\n", xRel, yRel);
		spawn.quantity--;
		if(spawn.quantity<=0){
			spawnQueue.pop_front();
		}
		//char* teName=new char[spawn.eName.length()+2];
		//strcpy(teName, spawn.eName.c_str());
		EDrawable* newEnt=Ents::CreateEntity(const_cast<char*>(spawn.eName.c_str()));
		//delete[] teName;
		EBaseNPC* newNPC=dynamic_cast<EBaseNPC*>(newEnt);
		if(newNPC==NULL){
			delete newEnt;
			return;
		}
		newNPC->SetSpawnedFrom(this);
		newNPC->SetPosition(xCell, yCell);
		newNPC->SetRelativePosition(xCell*TILE_SIZE_X-xRel, yCell*TILE_SIZE_Y-yRel);
		newNPC->SetPath(toHq);
		newNPC->SetTargetEnt(targetEnt);
		parentRoom->AddDrawable(newEnt);
	}
}

void ESpawnPoint::CreatePath(){
	std::list<EDrawable*>::iterator it;
	Pos2D thispos=Pos2D(GetXPos(), GetYPos());
	Pos2D finpos;
	EDrawable* hq=NULL;
	float dist=1000.0;
	for(it=parentRoom->Drawables.begin(); it!=parentRoom->Drawables.end(); it++){
		if((*it)->className=="ent_hq"){
			Pos2D pos=Pos2D((*it)->GetXPos(), (*it)->GetYPos());
			float ndist=thispos.Distance(pos);
			if(ndist<dist){
				hq=(*it);
				finpos=pos;
				targetEnt=hq;
				dist=ndist;
			}
		}
	}
	PFind::PathData* pData = new PFind::PathData();
	pData->pathStart=thispos;
	pData->pathEnd=finpos;
	pData->pathRoom=parentRoom;
	pData->pSize=Pos2D(2,2);
	bool success = PFind::FindPathS(true, Pos2D(2,2), pData);
	if(!success){
		canFind=false;
		return;
	}
	toHq=PFind::GetPathDS(Pos2D(2,2), pData);
	canFind=true;
	delete pData;
}

void ESpawnPoint::PathFound(MutexPData* mpData){
	SDL_mutexP(mpData->lock);
	printf("FPData: %p\n", mpData);
	std::list<EDrawable*>::iterator it;
	for(it=parentRoom->Drawables.begin(); it!=parentRoom->Drawables.end(); it++){
		EBaseNPC* npc=dynamic_cast<EBaseNPC*>((*it));
		if(!npc){
			continue;
		}
		if(npc->GetTargetEnt()==desHq){
			PFind::SetStart(Pos2D(npc->GetXPos(), npc->GetYPos()), mpData->pData);
			deque<Pos2D> toNewHq=PFind::GetPathDS(Pos2D(2,2), mpData->pData);
			npc->SetPath(toNewHq);
			npc->SetTargetEnt(newHq);
			npc->SetAttacking(NULL);
		}
	}
	SDL_mutexV(mpData->lock);
	SDL_WaitThread(pathFindThread, NULL);
	delete mpData->pData;
	SDL_DestroyMutex(mpData->lock);
	delete mpData;
	pathFindThread=NULL;
}

int ThreadFindPath(void* vpData){
	printf("Running Thread\n");
	MutexPData* mpData = static_cast<MutexPData*>(vpData);
	SDL_mutexP(mpData->lock);
	printf("TPData: %p\n", mpData->lock);
	PFind::FindPathS(false, Pos2D(2,2), mpData->pData);
	SDL_Event pEvent;
	pEvent.type=SDL_USEREVENT;
	pEvent.user.code=2;
	pEvent.user.data1=vpData;
	SDL_PushEvent(&pEvent);
	SDL_mutexV(mpData->lock);
	return 0;
}

void ESpawnPoint::DestroyedEnt(EHeadQuarters* eHq){
	std::list<EDrawable*>::iterator it;
	EDrawable* newAtk=NULL;
	float dist=1000.0;
	Pos2D thispos=Pos2D(eHq->GetXPos(), eHq->GetYPos());
	for(it=parentRoom->Drawables.begin(); it!=parentRoom->Drawables.end(); it++){
		if((*it)->className=="ent_hq"&&(*it)!=eHq){
			Pos2D pos=Pos2D((*it)->GetXPos(), (*it)->GetYPos());
			float ndist=thispos.Distance(pos);
			if(ndist<dist){
				newAtk=(*it);
				dist=ndist;
			}
		}
	}
	if(!newAtk){
		return;
	}
	MutexPData* mpData = new MutexPData();
	mpData->lock=SDL_CreateMutex();
	mpData->pData = new PFind::PathData();
	PFind::SetRoom(parentRoom, mpData->pData);
	PFind::SetEnd(Pos2D(newAtk->GetXPos(), newAtk->GetYPos()), mpData->pData);

	printf("Creating Thread\n");
	pathFindThread=SDL_CreateThread(ThreadFindPath, mpData);
	SDL_mutexP(mpData->lock);
	printf("CPData: %p\n", mpData->lock);
	SDL_mutexV(mpData->lock);

	//PFind::FindPathS(false, Pos2D(2,2), pData);
	newHq=newAtk;
	desHq=eHq;

}

WinCondition ESpawnPoint::WinConditionState(){
	int count=spawnQueue.size();
	if(count>0){
		return WIN_STAY;
	} else {
		return WIN_NULL;
	}
}