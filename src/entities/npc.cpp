#include "entities/manager.h"
#include "entities/npcs.h"
#include "graphics/window.h"
#include "room/room.h"
#include "util/timem.h"
#include <cmath>

using namespace Ents;
using namespace std;

const bool EBaseNPC::Purchasable=false;
const int EBaseNPC::Price=0;
const Pos2D EBaseNPC::pSize=Pos2D(1,1);

SpriteSheet* fireballSheet=NULL;

static EManEntry<EBaseNPC> ent_fireball("ent_fireball");

void EBaseNPC::Draw(){
	int xPos=floor(xRel);
	int yPos=floor(yRel);
	GraphicsM::DrawSprite(xPos, yPos-19, spriteNo, fireballSheet);

	std::list<DamageFloat>::iterator it;
	for(it=damageFloats.begin(); it!=damageFloats.end(); it++){
		GraphicsM::DrawTText(it->pos.x, it->pos.y, FontMan::GetFont("sm_text"), it->dmgText, 0, it->col);
	}
	//GraphicsM::DrawRectangle(xCell*TILE_SIZE_X, yCell*TILE_SIZE_Y, TILE_SIZE_X, TILE_SIZE_Y, Colour(100,100,100,100));
}

void EBaseNPC::Event(SDL_Event &kevent){

}

void EBaseNPC::Think(){
	if(parentRoom->GetStage()==STAGE_SCORE){
		return;
	}
	unsigned int Now=SDL_GetTicks();
	std::list<DamageFloat>::iterator it=damageFloats.begin();
	while(it!=damageFloats.end()){
		if(Now>it->TimeCreated+it->Life){
			delete it->dmgText;
			damageFloats.erase(it++);
			continue;
		}
		int alpha=it->col.a-200*Util::FrameTime();
		if(alpha>=0){
			it->col.a=alpha;
		}
		it->pos.y-=50*Util::FrameTime();
		++it;
	}

	float dX, dY;
	dX=targetX-xRel;
	dY=targetY-yRel;
	if(abs(dX)<1.0){
		dX=0.0;
	}
	if(abs(dY)<1.0){
		dY=0.0;
	}
	if(dX>0){
		spriteNo=1;
	} else if(dX<0) {
		spriteNo=2;
	}
	if(dY>0){
		spriteNo=3;
	} else if(dY<0){
		spriteNo=4;
	}
	if(abs(dX)+abs(dY)<1.0){
		GotoNextCell();
		return;
	}
	xRel=xRel+Util::Sign(dX)*Util::FrameTime()*20.0;
	yRel=yRel+Util::Sign(dY)*Util::FrameTime()*20.0;
	Pos2D cent = parentRoom->GetCell(xRel+32, yRel+22);
	xCell=cent.x;
	yCell=cent.y;
}

void EBaseNPC::SetPosition(int x, int y){
	xCell=x;
	yCell=y;
	targetX=xCell*TILE_SIZE_X;
	targetY=yCell*TILE_SIZE_Y;
}

bool EBaseNPC::StateChange(RoomStage newStage){
	return true;
}

EBaseNPC::EBaseNPC(){
	xRel=0;
	yRel=0;
	spriteNo=1;
	Moveable=false;
	if(!fireballSheet){
		fireballSheet=new SpriteSheet("contents/fire_sheet.png", 64, 64);
	}
	DrawOrder=1;
	lastAttack=0;
	attacking=NULL;
	health=200;
}

bool EBaseNPC::IsOccupy(int x, int y){
	return false;
}

Sprite EBaseNPC::GetSprite(){
	Sprite spr;
	return spr;
}

void EBaseNPC::SetSpawnedFrom(ESpawnPoint* spawner){
	spawnedFrom=spawner;
}

void EBaseNPC::SetRelativePosition(float x, float y){
	xRel=x;
	yRel=y;
}

void EBaseNPC::SetPath(deque<Pos2D> &path){
	toHq=path;
	//printf("Count: %d\n", toHq.size());
}

void EBaseNPC::GotoNextCell(){
	if(toHq.size()<=0){
		return;
	}
	Pos2D nextPos=toHq.front();
	if(SDL_GetTicks()>lastAttack+3000){
		EDrawable* entAt=parentRoom->GetEntity(nextPos.x, nextPos.y);
		if(entAt){
			float dX, dY;
			dX=entAt->GetXPos()*TILE_SIZE_X-xRel;
			dY=entAt->GetYPos()*TILE_SIZE_Y-yRel;
			if(abs(dX)<1.0){
				dX=0.0;
			}
			if(abs(dY)<1.0){
				dY=0.0;
			}
			if(dX>0){
				spriteNo=1;
			} else if(dX<0) {
				spriteNo=2;
			}
			if(dY>0){
				spriteNo=3;
			} else if(dY<0){
				spriteNo=4;
			}

			EHeadQuarters* hqAtk=dynamic_cast<EHeadQuarters*>(entAt);
			if(!hqAtk){
				//Do something here.
				return;
			}
			attacking=entAt;
			lastAttack=SDL_GetTicks();
			hqAtk->Attack(20, this);
			return;
		} else {
			attacking=NULL;
		}
	}
	if(!attacking){
		targetX=nextPos.x*TILE_SIZE_X;
		targetY=nextPos.y*TILE_SIZE_Y;
		toHq.pop_front();
	}
}

void EBaseNPC::SetAttacking(EDrawable* attack){
	attacking=attack;
}

void EBaseNPC::SetTargetEnt(EDrawable* target){
	targetEnt=target;
}

EDrawable* EBaseNPC::GetTargetEnt(){
	return targetEnt;
}

ESpawnPoint* EBaseNPC::GetSpawnedFrom(){
	return spawnedFrom;
}

WinCondition EBaseNPC::WinConditionState(){
	return WIN_LOSE;
}

void EBaseNPC::Attack(int damage){
	health-=damage;
	parentRoom->AddDamageToEnemy(damage);
	if(health<=0){
		parentRoom->RemoveDrawable(this, true);
		parentRoom->AddNPCKilled(1);
		parentRoom->AddMoneyGained(100);
		return;
	}

	DamageFloat newDmg;
	newDmg.col=Colour(100,255,100,255);
	float randX=float(rand())/float(RAND_MAX);
	float randY=float(rand())/float(RAND_MAX);
	unsigned int Now=SDL_GetTicks();
	newDmg.TimeCreated=Now;
	newDmg.Life=1500;
	newDmg.pos=Pos2D(xRel+randX*64, yRel-randY*10);
	char* dmgText=new char[10];
	sprintf(dmgText, "-%d", damage);
	newDmg.dmgText=dmgText;
	damageFloats.push_back(newDmg);
}