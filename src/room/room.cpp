#include "room/room.h"
#include "entities/manager.h"
#include "modules/game.h"
#include "pathfind.h"

#include <list>
#include <cmath>

using namespace std;

SpriteSheet *tiles=0;

//Texture manager? maybe.
SpriteSheet *headQuartersSheet=NULL;


TRoom::TRoom(int w, int h){
	if(tiles==0){
		tiles=new SpriteSheet("contents/tiles.png", 64, 45);
	}
	spriteNo=TILE_TILED;
	sizeW=w;
	sizeH=h;
	parentMod=NULL;
	rmStage=STAGE_BUILD;
	selectedEnt=NULL;

	transitioning=true;
	tranStart=SDL_GetTicks();
	centerCell=Pos2D((sizeW/2)*TILE_SIZE_X, (sizeH/2)*TILE_SIZE_Y);
}

TRoom::~TRoom(){
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		delete (*it);
	}
}

void TRoom::AddDrawable(EDrawable* drawEnt){
	Drawables.push_back(drawEnt);
	drawEnt->parentRoom=this;
}

void TRoom::RemoveDrawable(EDrawable* drawEnt, bool safe){
	if(safe){
		toDelete.push_back(drawEnt);
	} else {
		Drawables.remove(drawEnt);
	}
}

void TRoom::SetTileStyle(TileStyle tileSty){
	spriteNo=tileSty;
}

bool compare_EDrawableP(EDrawable* draw1, EDrawable *draw2){
	int y1, y2;
	y1=draw1->GetYPos();
	y2=draw2->GetYPos();
	if(y1<y2) return true;
	if(y1>y2) return false;
	if(y1==y2){
		int d1, d2;
		d1=draw1->GetDrawOrder();
		d2=draw2->GetDrawOrder();
		if(d1<d2){
			return true;
		} else {
			return false;
		}
	}
	return false;
}

void TRoom::Draw(){
	int x,y=0;
	//GraphicsM::DrawRectangle(0, -100, sizeW*64, 100, Colour(100,100,100,255));
	float tranStage=0.0;
	if(transitioning){
		unsigned int Now=SDL_GetTicks();
		tranStage=(Now-tranStart)/2000.0;
		if(tranStage>1.0){
			transitioning=false;
		}
		//printf("tranStage: %f\n", tranStage);
	}
	for(y=0;y<sizeH;y++){
		for(x=0;x<sizeW;x++){
			if(transitioning){
				float distance=Pos2D(x*TILE_SIZE_X, y*TILE_SIZE_Y).Distance(centerCell);
				float alpha=((tranStage*2)+0.5)*255.0-(distance/5.0);
				if(alpha>255.0) alpha=255.0;
				if(alpha<150.0) alpha=0.0;
				GraphicsM::DrawSprite(x*TILE_SIZE_X, y*TILE_SIZE_Y, spriteNo, tiles, Colour(255,255,255,alpha));
				
			} else {
				GraphicsM::DrawSprite(x*TILE_SIZE_X, y*TILE_SIZE_Y, spriteNo, tiles);
			}
		}
	}
	std::list<EDrawable*> toDraw;
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		toDraw.push_back((*it));
	}
	toDraw.sort(compare_EDrawableP);
	if(transitioning){
		GraphicsM::AlphaOverride(true);
		GraphicsM::SetAlphaMultiplier(tranStage);
	}
	std::list<EDrawable*>::iterator it2;
	for(it2=toDraw.begin();it2!=toDraw.end();it2++){
		(*it2)->Draw();
		/*int y2=(*it2)->GetYPos();
		int d2=(*it2)->GetDrawOrder();
		char* name=(*it2)->className;
		printf("Y: %d, D: %d C: %s\n", y2, d2, name);*/
	}
	if(transitioning){
		GraphicsM::AlphaOverride(false);
	}
}

int TRoom::GetTotalSizeX(){
	return sizeW*TILE_SIZE_X;
}

int TRoom::GetTotalSizeY(){
	return sizeH*TILE_SIZE_Y;
}

int TRoom::GetSizeX(){
	return sizeW;
}

int TRoom::GetSizeY(){
	return sizeH;
}

Pos2D TRoom::GetCell(int x, int y){
	return Pos2D(floor((float)x/(float)TILE_SIZE_X), floor((float)y/(float)TILE_SIZE_Y));
}

void TRoom::Event(SDL_Event &kevent){
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		(*it)->Event(kevent);
	}
	if(kevent.type==SDL_MOUSEBUTTONUP){
		Pos2D panPos;
		if(parentMod){
			panPos=parentMod->GetPanPos();
		}
		int mX = kevent.button.x-panPos.x;
		int mY = kevent.button.y-panPos.y;
		Pos2D cellPos=GetCell(mX, mY);
		printf("MX: %d  MY: %d\n", cellPos.x, cellPos.y);
		if(GetStage()==STAGE_GAME){
			if(selectedEnt){
				selectedEnt->OnDeselect();
			}
			selectedEnt=GetEntity(cellPos.x, cellPos.y);
			if(selectedEnt){
				selectedEnt->OnSelect();
			}
		}
	}
}

Pos2D TRoom::GetHoverCell(){
	int mX, mY;
	SDL_GetMouseState(&mX, &mY);
	Pos2D panPos=parentMod->GetPanPos();
	Pos2D mPos=Pos2D(mX, mY)-panPos;
	return GetCell(mPos.x, mPos.y);
}

Pos2D TRoom::GetMouseCell(Pos2D mPos){
	Pos2D panPos=parentMod->GetPanPos();
	mPos=mPos-panPos;
	return GetCell(mPos.x, mPos.y);
}

void TRoom::Think(){
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		(*it)->Think();
	}
	int deleted=0;
	for(it=toDelete.begin();it!=toDelete.end();it++){
		Drawables.remove((*it));
		delete (*it);
		deleted++;
	}
	if(deleted>0){
		toDelete.clear();
		CheckWin();
	}
}

bool TRoom::IsOccupied(int x, int y, EDrawable* filter){
	if(x>=sizeW||x<0){
		return true;
	}
	if(y>=sizeH||y<0){
		return true;
	}
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		//if((*it)==filter){
			//continue;
		//}
		if((*it)->IsOccupy(x, y)){
			return true;
			break;
		}
	}
	return false;
}

bool TRoom::IsOccupied(Pos2D pos, Pos2D size, EDrawable* filter){
	if(pos.x+size.x>=sizeW||pos.x<0){
		return true;
	}
	if(pos.y+size.y>=sizeH||pos.y<0){
		return true;
	}
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		if((*it)==filter){
			continue;
		}
		for(int x=0;x<size.x;x++){
			for(int y=0;y<size.y;y++){
				if((*it)->IsOccupy(pos.x+x, pos.y+y)){
				return true;
				}
			}
		}
	}
	return false;
}

EDrawable* TRoom::GetEntity(int x, int y){
	if(x>=sizeW||x<0){
		return NULL;
	}
	if(y>=sizeH||y<0){
		return NULL;
	}
	std::list<EDrawable*>::iterator it;
	for(it=Drawables.begin();it!=Drawables.end();it++){
		if((*it)->IsOccupy(x, y)){
			return *it;
			break;
		}
	}
	return NULL;
}

bool TRoom::IsOccupied(int x, int y){
	return IsOccupied(x, y, NULL);
}

void TRoom::SetGameModu(GameModu* set){
	parentMod=set;
}

void TRoom::DrawBorder(Pos2D cell){
	int x=TILE_SIZE_X*cell.x;
	int y=TILE_SIZE_Y*cell.y;
	Colour col=Colour(100, 255, 100, 255);
	if(IsOccupied(cell.x, cell.y)){
		col=Colour(255, 100, 100, 255);
	}
	GraphicsM::DrawTLine(x, y, x+TILE_SIZE_X, y, col);
	GraphicsM::DrawTLine(x, y, x, y+TILE_SIZE_Y, col);
	GraphicsM::DrawTLine(x+TILE_SIZE_X, y, x+TILE_SIZE_X, y+TILE_SIZE_Y, col);
	GraphicsM::DrawTLine(x, y+TILE_SIZE_Y, x+TILE_SIZE_X, y+TILE_SIZE_Y, col);
	//GraphicsM::DrawRectangle(x, y, TILE_SIZE_X, TILE_SIZE_Y, col);
	//printf("X: %d Y: %d\n", x, y);
}

void TRoom::DrawBorder(Pos2D cell, Colour col){
	int x=TILE_SIZE_X*cell.x;
	int y=TILE_SIZE_Y*cell.y;
	GraphicsM::DrawTLine(x, y, x+TILE_SIZE_X, y, col);
	GraphicsM::DrawTLine(x, y, x, y+TILE_SIZE_Y, col);
	GraphicsM::DrawTLine(x+TILE_SIZE_X, y, x+TILE_SIZE_X, y+TILE_SIZE_Y, col);
	GraphicsM::DrawTLine(x, y+TILE_SIZE_Y, x+TILE_SIZE_X, y+TILE_SIZE_Y, col);
	//GraphicsM::DrawRectangle(x, y, TILE_SIZE_X, TILE_SIZE_Y, col);
	//printf("X: %d Y: %d\n", x, y);
}

GameModu* TRoom::GetGameModu(){
	return parentMod;
}

void TRoom::SetStage(RoomStage setStage){
	bool success=true;
	list<EDrawable*>::iterator it;
	for(it=Drawables.begin(); it!=Drawables.end(); it++){
		bool willchange=(*it)->StateChange(setStage);
		if(!willchange){
			success=false;
		}
	}
	if(success){
		rmStage=setStage;
		parentMod->NewStage(setStage);
	}
}

RoomStage TRoom::GetStage(){
	return rmStage;
}

void TRoom::RecalculateCounts(){
	entCounts.clear();
	list<EDrawable*>::iterator it;
	for(it=Drawables.begin(); it!=Drawables.end(); it++){
		int elements=entCounts.count((*it)->className);
		if(elements<=0){
			entCounts[(*it)->className]=1;
		} else {
			entCounts[(*it)->className]++;
		}
	}
}

void TRoom::ClearAllEnts(){
	list<EDrawable*>::iterator it;
	for(it=Drawables.begin(); it!=Drawables.end(); it++){
		delete (*it);
	}
	Drawables.clear();
}

void TRoom::SetupLevel(Util::LevelData* lData){
	sizeW=lData->size.x;
	sizeH=lData->size.y;
	levelData=lData;
	vector<Util::EntData>::iterator it;
	for(it=lData->entTable.begin(); it<lData->entTable.end(); it++){
		char entName[100];
		strcpy(entName, it->eName.c_str());
		EDrawable* newEnt=Ents::CreateEntity(entName);
		newEnt->parentRoom=this;
		newEnt->SetPosition(it->position.x, it->position.y);
		newEnt->StringParam(it->excess);
		AddDrawable(newEnt);
	}
	RecalculateCounts();
	centerCell=Pos2D((sizeW/2)*TILE_SIZE_X, (sizeH/2)*TILE_SIZE_Y);
}

const char* TRoom::GetLevelName(){
	return levelData->fileName.c_str();
}

int TRoom::GetCount(const std::string eName){
	map<std::string, int>::iterator it=entCounts.find(eName);
	if(it==entCounts.end()){
		return 0;
	}
	return it->second;
}

int TRoom::GetLimit(const std::string eName){
	map<std::string, int>::iterator it=levelData->eLimits.find(eName);
	if(it==levelData->eLimits.end()){
		return 0;
	}
	return it->second;
}

void TRoom::CheckWin(){
	if(rmStage==STAGE_SCORE){
		return;
	}
	list<EDrawable*>::iterator it;
	int wins=0;
	int stays=0;
	int losses=0;
	for(it=Drawables.begin(); it!=Drawables.end(); it++){
		WinCondition wCond=(*it)->WinConditionState();
		if(wCond==WIN_WIN){
			wins++;
		} else if(wCond==WIN_STAY){
			stays++;
		} else if(wCond==WIN_LOSE){
			losses++;
		}
	}
	if(stays>0){
		return;
	}
	if(wins>0&&losses>0){
		return;
	}
	SetStage(STAGE_SCORE);
}

EDrawable* TRoom::GetSelectedEnt(){
	return selectedEnt;
}

void TRoom::AddNPCKilled(int add){
	roomScore.NPCsKilled+=add;
}

void TRoom::AddDamageToEnemy(int add){
	roomScore.DamageOnEnemy+=add;
}

void TRoom::AddDamageToFriendly(int add){
	roomScore.DamageOnFriendly+=add;
}

void TRoom::AddMoneyGained(int add){
	roomScore.Money+=add;
}

RoomScoreData TRoom::GetScoreData(){
	return roomScore;
}