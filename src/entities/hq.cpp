#include "entities/manager.h"
#include "entities/npcs.h"
#include "graphics/window.h"
#include "room/room.h"
#include "profiles.h"

using namespace Profiles;
using namespace Ents;

SpriteSheet* hqSprite=NULL;
SpriteSheet* hqSpriteIcon=NULL;

const bool EHeadQuarters::Purchasable=true;
const int EHeadQuarters::Price=500;
const Pos2D EHeadQuarters::pSize=Pos2D(2,2);

static EManEntry<EHeadQuarters> ent_hq("ent_hq");

void EHeadQuarters::Draw(){
	int pX, pY;
	pX=xCell*TILE_SIZE_X;
	pY=yCell*TILE_SIZE_Y;
	GraphicsM::DrawSprite(pX, pY-38, spriteNo, hqSprite);
	float hpPerc=float(Health)/2000.0;
	Colour col;
	col.r=(1-hpPerc)*255.0;
	col.g=hpPerc*255.0;
	col.b=0;
	col.a=255;
	GraphicsM::DrawRectangle(pX, pY-48, hqSprite->GetCellWi(), 12, Colour(100,100,100,100));
	GraphicsM::DrawRectangle(pX+2, pY-46, (hqSprite->GetCellWi()-4)*hpPerc, 8, col);
}

void EHeadQuarters::Event(SDL_Event &kevent){

}

void EHeadQuarters::Think(){

}

EHeadQuarters::EHeadQuarters(){
	if(!hqSprite){
		hqSprite=new SpriteSheet("contents/hqs.png", 128, 128);
	}
	Faction pFact=GetActiveProfile()->GetFaction();
	if(pFact==FACTION_FIRE_BALL){
		spriteNo=2;
		printf("Fireball...\n");
	} else if(pFact==FACTION_ICE_CREAM){
		spriteNo=1;
		printf("Ice Cream...\n");
	}
	Moveable=false;
	DrawOrder=0;
	Health=2000;
}

bool EHeadQuarters::IsOccupy(int x, int y){
	if((x==xCell||x==xCell+1)&&(y==yCell||y==yCell+1)) return true;
	return false;
}

Sprite EHeadQuarters::GetSprite(){
	if(!hqSpriteIcon){
		hqSpriteIcon=new SpriteSheet("contents/hqs_icon.png", 64, 64);
	}
	Sprite spr=Sprite(hqSpriteIcon,0);
	Faction pFact=GetActiveProfile()->GetFaction();
	if(pFact==FACTION_FIRE_BALL){
		spr.cellNo=2;
	}
	if(pFact==FACTION_ICE_CREAM){
		spr.cellNo=1;
	}
	return spr;
}

void EHeadQuarters::Attack(int damage, EBaseNPC* attacker){
	Health-=damage;
	parentRoom->AddDamageToFriendly(damage);
	//printf("Health: %d\n", Health);
	if(Health<=0){
		parentRoom->RemoveDrawable(this);
		parentRoom->CheckWin();
		attacker->GetSpawnedFrom()->DestroyedEnt(this);
		delete this;
	}
}

WinCondition EHeadQuarters::WinConditionState(){
	return WIN_WIN;
}