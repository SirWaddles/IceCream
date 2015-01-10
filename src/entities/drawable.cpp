#include "entities/entity.h"
#include <cmath>

using namespace std;

void EDrawable::Draw(){

}

void EDrawable::Event(SDL_Event &kevent){

}

void EDrawable::Think(){

}

EDrawable::EDrawable(){
	xCell=0;
	yCell=0;
	DrawOrder=0;
	Moveable=false;
}

float EDrawable::GetXPos(){
	return xCell;
}

float EDrawable::GetYPos(){
	return yCell;
}

/*int EDrawable::GetXCell(){
	int targetX=floor((float(xPos)/65.0)+0.5);
	return targetX;
}

int EDrawable::GetYCell(){
	int targetY=floor((float(yPos)/45.0)+0.5);
	return targetY;
}*/

int EDrawable::GetDrawOrder(){
	return DrawOrder;
}

bool EDrawable::IsOccupy(int x, int y){
	return false;
}

void EDrawable::SetPosition(int x, int y){
	xCell=x;
	yCell=y;
}

Sprite EDrawable::GetSprite(){
	return Sprite();
}

void EDrawable::StringParam(string str){

}

bool EDrawable::StateChange(RoomStage newStage){
	return true;
}

WinCondition EDrawable::WinConditionState(){
	return WIN_NULL;
}

void EDrawable::OnSelect(){

}

void EDrawable::OnDeselect(){

}

const bool EDrawable::Purchasable=false;
const int EDrawable::Price=0;
const Pos2D EDrawable::pSize=Pos2D(1,1);