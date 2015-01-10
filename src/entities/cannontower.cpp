#include "entities/manager.h"
#include "util/timem.h"
#include "graphics/window.h"
#include "room/room.h"

#include <cmath>

#define PI 3.14159265
#define NLE 2.71828183

using namespace Ents;

const bool ECannonTower::Purchasable=true;
const int ECannonTower::Price=500;
const Pos2D ECannonTower::pSize=Pos2D(1,1);

static EManEntry<ECannonTower> ent_cannontower("ent_cannontower");

SpriteSheet* cannonTower=NULL;
SpriteSheet* cannonTowerIcon=NULL;
Texture* targetSpr=NULL;


void ECannonTower::Draw(){
	if(parentRoom->GetSelectedEnt()==this&&parentRoom->GetStage()==STAGE_GAME){
		parentRoom->DrawBorder(Pos2D(xCell, yCell), Colour(100, 100, 255, 255));
		Pos2D hoverPos=parentRoom->GetHoverCell();
		int hX, hY;
		hX=hoverPos.x*TILE_SIZE_X;
		hY=hoverPos.y*TILE_SIZE_Y;
		GraphicsM::DrawTexturedRectangle(hX+9, hY, 45, 45, Colour(0,0,0,255), targetSpr);
		if(attacking){
			int aX, aY;
			aX=attackPos.x*TILE_SIZE_X;
			aY=attackPos.y*TILE_SIZE_Y;
			GraphicsM::DrawTexturedRectangle(aX+9, aY, 45, 45, Colour(255,255,255,255), targetSpr);
		}	
	}
	
	int cX, cY;
	cX=xCell*TILE_SIZE_X;
	cY=yCell*TILE_SIZE_Y;
	GraphicsM::DrawSprite(cX, cY-83, 1, cannonTower);
}

void ECannonTower::Event(SDL_Event &kevent){
	if(kevent.type==SDL_MOUSEBUTTONUP&&parentRoom->GetSelectedEnt()==this){
		attackPos=parentRoom->GetMouseCell(Pos2D(kevent.button.x, kevent.button.y));
		attacking=true;
		lastAttack=SDL_GetTicks();
	}
}

void ECannonTower::Think(){
	if(parentRoom->GetStage()==STAGE_GAME&&attacking){
		if(SDL_GetTicks()>lastAttack+5000){
			lastAttack=SDL_GetTicks();

			EDrawable* newBall=Ents::CreateEntity("ent_cannonball");
			ECannonBall* cBall=dynamic_cast<ECannonBall*>(newBall);

			float oX, oY;
			oX=float(attackPos.x*TILE_SIZE_X-xCell*TILE_SIZE_X);
			oY=float(attackPos.y*TILE_SIZE_Y-yCell*TILE_SIZE_Y);
			float offsetDeg=atan2(oY, oX);

			cBall->SetAttackPos(attackPos);
			cBall->SetOffset(offsetDeg);

			float dist=Pos2D(xCell*TILE_SIZE_X, yCell*TILE_SIZE_Y).Distance(Pos2D(attackPos.x*TILE_SIZE_X, attackPos.y*TILE_SIZE_Y));
			unsigned int endPoint=lastAttack+(dist*2);

			cBall->SetEndPoint(endPoint);
			cBall->SetDistance(dist);

			cBall->SetPosition(xCell, yCell);
			cBall->SetInitialPos(Pos2D(xCell, yCell));

			printf("Firing: %f %f %f\n", offsetDeg*180/PI, oX, oY);
			printf("CannonDistance: %f\n", dist);
			parentRoom->AddDrawable(cBall);
		}
	}
}

bool ECannonTower::IsOccupy(int x, int y){
	if(x==xCell&&y==yCell){
		return true;
	}
	return false;
}

ECannonTower::ECannonTower(){
	if(!cannonTower){
		cannonTower=new SpriteSheet("contents/cannon_tower.png", 64, 128);
	}
	if(!targetSpr){
		targetSpr=new Texture("contents/target.png");
	}
	
	attackPos=Pos2D(0,0);
	attacking=false;
	lastAttack=0;
}

Sprite ECannonTower::GetSprite(){
	if(!cannonTowerIcon){
		cannonTowerIcon=new SpriteSheet("contents/cannon_tower_icon.png", 64, 64);
	}
	Sprite spr;
	spr.mSheet=cannonTowerIcon;
	spr.cellNo=1;
	return spr;
}