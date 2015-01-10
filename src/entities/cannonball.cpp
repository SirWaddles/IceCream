#include "entities/manager.h"
#include "entities/npcs.h"
#include "room/room.h"
#include "graphics/window.h"
#include "util/particles.h"
#include <cmath>
#include <ctime>

const bool ECannonBall::Purchasable=false;
const int ECannonBall::Price=0;
const Pos2D ECannonBall::pSize=Pos2D(1,1);

static Ents::EManEntry<ECannonBall> ent_cannonball("ent_cannonball");

SpriteSheet* cannonBallSpr=NULL;
Texture* partDem=NULL;

void ECannonBall::Draw(){
	partSys.Draw();
	GraphicsM::DrawSprite(cannonPos.x, cannonPos.y, 1, cannonBallSpr);
}

void ECannonBall::Think(){
	unsigned int now=SDL_GetTicks();
	if(now<endPoint){
		float cannonLinMax=endPoint-lastAttack;
		float cannonLinPath=(now-lastAttack)/cannonLinMax;
		float cannonExpPath=cannonLinPath*cannonLinPath;

		float xPos, yPos;
		xPos=(cos(cannonOffset)*cannonDistance*cannonLinPath)+initialPos.x*TILE_SIZE_X;
		yPos=(sin(cannonOffset)*cannonDistance*cannonLinPath)+initialPos.y*TILE_SIZE_Y-(74*(1-cannonExpPath))-9;
		cannonPos.x=floor(xPos);
		cannonPos.y=floor(yPos);
	} 
	if(now>endPoint+2000){
		parentRoom->RemoveDrawable(this, true);
		//delete this;
		return;
	}
	if(now>endPoint-10&&!landed){
		landed=true;
		srand(time(NULL));
		float randto=float(rand())/float(RAND_MAX);
		int parts=floor(randto*100.0)+200;
		for(int i=0;i<parts;i++){
			Util::Particle* partEnt=new Util::Particle();
			partEnt->SetAlpha(255);
			partEnt->SetAlphaDecay(255.0);
			partEnt->SetColour(Colour(255,255,255,255));
			float randdir=float(rand())/float(RAND_MAX);
			partEnt->SetDirection(randdir*360.0);
			float randlife=float(rand())/float(RAND_MAX);
			partEnt->SetLife(randlife*1000.0);
			float randX=float(rand())/float(RAND_MAX);
			float randY=float(rand())/float(RAND_MAX);
			partEnt->SetPosition(cannonPos+Pos2D(randX*64.0, randY*64.0));
			partEnt->SetSize(8.0);
			partEnt->SetPEndSize(0.0);
			partEnt->SetSpin(5.0);
			float randVel=float(rand())/float(RAND_MAX);
			partEnt->SetVelocity(randVel*300.0);
			partEnt->SetVelDecay(100.0);
			partEnt->SetTexture(partDem);
			partSys.AddParticle(partEnt);
		}
		std::list<EDrawable*>::iterator it;
		//Pos2D curPos=Pos2D(xCell, yCell);
		for(it=parentRoom->Drawables.begin(); it!=parentRoom->Drawables.end(); it++){
			EBaseNPC* newNPC=dynamic_cast<EBaseNPC*>((*it));
			if(newNPC){
				if(cannonAttackPos==Pos2D((*it)->GetXPos(), (*it)->GetYPos())){
					float randDam=float(rand())/float(RAND_MAX);
					newNPC->Attack(50+randDam*20);
				}
			}
		}
	}
	partSys.Think();
	Pos2D cent = parentRoom->GetCell(cannonPos.x+32, cannonPos.y+22);
	xCell=cent.x;
	yCell=cent.y;
}

bool ECannonBall::IsOccupy(int x, int y){
	return false;
}

ECannonBall::ECannonBall(){
	if(!cannonBallSpr){
		cannonBallSpr=new SpriteSheet("contents/cannonball.png", 64, 64);
	}
	if(!partDem){
		partDem=new Texture("contents/part.png");
	}
	lastAttack=SDL_GetTicks();
	landed=false;
	DrawOrder=3;
}

void ECannonBall::SetAttackPos(Pos2D attack){
	cannonAttackPos=attack;
}

void ECannonBall::SetEndPoint(unsigned int end){
	endPoint=end;
}

void ECannonBall::SetOffset(float off){
	cannonOffset=off;
}

void ECannonBall::SetDistance(float distance){
	cannonDistance=distance;
}

Sprite ECannonBall::GetSprite(){
	Sprite spr;
	return spr;
}

void ECannonBall::SetInitialPos(Pos2D pos){
	initialPos=pos;
}