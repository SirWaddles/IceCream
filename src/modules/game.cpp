#include "modules/game.h"
#include "room/room.h"
#include "entities/manager.h"
#include "panels/panel.h"

#include "modules/scoreboard.h"

#include "profiles.h"
#include "util/leveldata.h"
#include <cmath>

using namespace GraphicsM;

DButton* startButton=NULL;

void StartGame(DPanel* button, void* data){
	TRoom* activeRoom=static_cast<TRoom*>(data);
	if(activeRoom){
		activeRoom->SetStage(STAGE_GAME);
	}
}

void GameModu::Initialize(){
	activeRoom=new TRoom(20, 20);
	activeRoom->SetGameModu(this);
	//activeRoom->SetupLevel(Util::GetLevelData("level1"));
	const char* levelName=Profiles::GetActiveProfile()->GetNextLevel();
	activeRoom->SetupLevel(Util::GetLevelData(levelName));
	centerX=((float)(activeRoom->GetTotalSizeX()-ScrW()))*0.5;
	centerY=((float)(activeRoom->GetTotalSizeY()-ScrH()))*0.5;
	panX=-centerX;
	panY=-centerY;
	panning=false;
	viewPX=0;
	viewPY=0;
	mouseEntity=NULL;

	transitioning=true;
	tranStart=SDL_GetTicks();
	
	startButton=new DButton(ScrW()-100, ScrH()-90, 90);
	startButton->SetHeight(80);
	startButton->SetText("Start");
	startButton->SetAction(StartGame);
	startButton->SetDataPointer(activeRoom);

	closeTrans=NULL;

	NewStage(STAGE_BUILD);
}

void GameModu::Clean(){
	printf("Destruction check\n");
	delete activeRoom;
	delete startButton;
	activeRoom=NULL;
}

void GameModu::Draw(){
	ClearWindow(Colour(0, 0, 0));
	int mX, mY;
	SDL_GetMouseState(&mX, &mY);
	ResetOrigin(panX, panY);
	activeRoom->Draw();
	if(mouseEntity){
		Pos2D eSize=Ents::GetEntSize(mouseEntity);
		for(int x=0;x<eSize.x;x++){
			for(int y=0;y<eSize.y;y++){
				Pos2D cell=activeRoom->GetCell(mX-panX, mY-panY)+Pos2D(x,y);
				activeRoom->DrawBorder(cell);
			}
		}
	}
	ResetOrigin(-panX, -panY);
	if(transitioning){
		ResetOrigin(0, 100*(1-tranStage));
	}
	if(activeRoom->GetStage()==STAGE_BUILD){
		GraphicsM::DrawRectangle(0, ScrH()-100, ScrW(), 100, Colour(100,100,100,255));
		map<string, Ents::IManEntry*>::iterator it;
		int x=50;
		for(it=Ents::GetEMap().begin(); it!=Ents::GetEMap().end(); it++){
			if(!it->second->IsPurchasable()){
				continue;
			}
			Sprite eSpr=it->second->GetSprite();
			if((x+eSpr.mSheet->GetCellWi()>ScrW())){
				break;
			}
			float sprH=eSpr.mSheet->GetCellHi();
			sprH*=0.5;
			sprH+=50;
			GraphicsM::DrawRectangle(x, ScrH()-sprH, eSpr.mSheet->GetCellWi(), eSpr.mSheet->GetCellHi(), Colour(150,150,150,255));
			GraphicsM::DrawSprite(x, ScrH()-sprH, eSpr.cellNo, eSpr.mSheet);
			if(activeRoom->GetCount(it->first)>=activeRoom->GetLimit(it->first)){
				GraphicsM::DrawRectangle(x, ScrH()-sprH, eSpr.mSheet->GetCellWi(), eSpr.mSheet->GetCellHi(), Colour(0,0,0,100));
			}
			x+=eSpr.mSheet->GetCellWi()+10;
		}
		GraphicsM::DrawTText(20, ScrH()-100, FontMan::GetFont("sm_text"), "Avaliable Entities:", 0, Colour(255,255,255,255));
		startButton->DoDraw();
	}
	if(transitioning){
		ResetOrigin(0, -100*(1-tranStage));
	}
	if(mouseEntity){
		Sprite mSpr=Ents::GetEntSprite(mouseEntity);
		int centX, centY;
		centX=floor((float)mSpr.mSheet->GetCellWi()*0.5);
		centY=floor((float)mSpr.mSheet->GetCellHi()*0.5);
		
		GraphicsM::DrawSprite(mX-centX, mY-centY, mSpr.cellNo, mSpr.mSheet);
	}
	if(closeTrans){
		closeTrans->Draw();
	}
}

void GameModu::Think(){
	if(transitioning){
		unsigned int Now=SDL_GetTicks();
		tranStage=float(Now-tranStart)/1000.0;
		if(tranStage>=1.0){
			transitioning=false;
		}
	}
	activeRoom->Think();
	if(panning){
		int mX, mY;
		SDL_GetMouseState(&mX, &mY);
		panX=oPanX-(oClickX-mX);
		panY=oPanY-(oClickY-mY);
		//printf("PanX: %d  PanY: %d\n", panX, panY);
		ResetPan();
	}
	startButton->TopThink();
	if(closeTrans){
		closeTrans->Think();
		if(closeTrans->Ready()){
			SDL_Event deleteEvent;
			deleteEvent.type=SDL_USEREVENT;
			deleteEvent.user.code=3;
			deleteEvent.user.data1=closeTrans;
			deleteEvent.user.data2=this;
			SDL_PushEvent(&deleteEvent);
			printf("Sending Event\n");
		}
	}
}

void GameModu::ResetPan(){
	int maxX=(ScrW()-viewPX)-activeRoom->GetTotalSizeX();
	int maxY=(ScrH()-viewPY)-activeRoom->GetTotalSizeY();
	if(activeRoom->GetTotalSizeX()<(ScrW()-viewPX)){
		if(panX<0) panX=0;
		if(panX>maxX) panX=maxX;
	} else {
		if(panX>0) panX=0;
		if(panX<maxX) panX=maxX;
	}
	if(activeRoom->GetTotalSizeY()<(ScrH()-viewPY)){
		if(panY<0) panY=0;
		if(panY>maxY) panY=maxY;
	} else {
		if(panY>0) panY=0;
		if(panY<maxY) panY=maxY;
	}
}

void GameModu::BGThink(){

}

void GameModu::Event(SDL_Event &kevent){
	if(kevent.type==SDL_KEYUP){
		if(kevent.key.keysym.sym==SDLK_ESCAPE){
			GMods::SetFocus(GMods::GetModule("menu"));
		}
	}
	if(kevent.type==SDL_MOUSEBUTTONUP){
		if(kevent.button.button==SDL_BUTTON_RIGHT){
			panning=false;
		}
	}
	if(kevent.type==SDL_MOUSEBUTTONDOWN){
		if(kevent.button.button==SDL_BUTTON_RIGHT){
			panning=true;
			oClickX=kevent.button.x;
			oClickY=kevent.button.y;
			oPanX=panX;
			oPanY=panY;
		}
	}
	if(kevent.type==SDL_VIDEORESIZE){
		ResetPan();
		startButton->SetX(ScrW()-100);
		startButton->SetY(ScrH()-90);
	}
	if(activeRoom->GetStage()==STAGE_BUILD&& kevent.type==SDL_MOUSEBUTTONDOWN &&kevent.button.button==SDL_BUTTON_LEFT&&!mouseEntity){
		if(kevent.button.y>ScrH()-100){
			map<string, Ents::IManEntry*>::iterator it;
			int x=50;
			for(it=Ents::GetEMap().begin(); it!=Ents::GetEMap().end(); it++){
				if(!it->second->IsPurchasable()){
					continue;
				}
				Sprite eSpr=it->second->GetSprite();
				if((x+eSpr.mSheet->GetCellWi()>ScrW())){
					break;
				}
				float sprH=eSpr.mSheet->GetCellHi();
				sprH*=0.5;
				sprH+=50;
				int y=ScrH()-sprH;
				int mX, mY;
				mX=kevent.button.x;
				mY=kevent.button.y;
				printf("Detecting...\n");
				printf("MX: %d  MY: %d\n", mX, mY);
				printf("X: %d  Y: %d\n", x, y);
				printf("W: %d  Y: %d\n", eSpr.mSheet->GetCellWi(), eSpr.mSheet->GetCellHi());
				if(mX>x&&mX<x+eSpr.mSheet->GetCellWi()&&mY>y&&mY<y+eSpr.mSheet->GetCellHi()){
					printf("Now has: %d\n", activeRoom->GetCount(it->first));
					if(activeRoom->GetCount(it->first)<activeRoom->GetLimit(it->first)){
						mouseEntity=new char[50];
						strcpy(mouseEntity,it->first.c_str());
						printf("Making %s\n", mouseEntity);
					}
				}
				x+=eSpr.mSheet->GetCellWi()+10;
			}
		} else {
			EDrawable* selEnt=NULL;
			int mX, mY;
			mX=kevent.button.x-panX;
			mY=kevent.button.y-panY;
			Pos2D cellPos=activeRoom->GetCell(mX, mY);
			selEnt=activeRoom->GetEntity(cellPos.x, cellPos.y);
			if(selEnt&&selEnt->Moveable){
				mouseEntity=new char[50];
				strcpy(mouseEntity, selEnt->className);
				activeRoom->RemoveDrawable(selEnt);
				delete selEnt;
			}
		}
	}
	if(activeRoom->GetStage()==STAGE_BUILD&& kevent.type==SDL_MOUSEBUTTONUP &&kevent.button.button==SDL_BUTTON_LEFT&&mouseEntity){
		int mX, mY;
		printf("Made\n");
		mX=kevent.button.x-panX;
		mY=kevent.button.y-panY;
		Pos2D cellPos=activeRoom->GetCell(mX, mY);
		Pos2D eSize=Ents::GetEntSize(mouseEntity);
		if(!activeRoom->IsOccupied(cellPos, eSize, NULL)){
			if(activeRoom->GetCount(mouseEntity)<activeRoom->GetLimit(mouseEntity)){
				EDrawable* newEnt=Ents::CreateEntity(mouseEntity);
				newEnt->SetPosition(cellPos.x, cellPos.y);
				newEnt->Moveable=true;
				activeRoom->AddDrawable(newEnt);
				activeRoom->RecalculateCounts();
			}
		}
		delete[] mouseEntity;
		mouseEntity=NULL;
	}

	if(kevent.type==SDL_USEREVENT&&kevent.user.code==3){
		printf("Got event\n");
		GameModu* tMod=static_cast<GameModu*>(kevent.user.data2);
		Util::SteelTransition* tTrans=static_cast<Util::SteelTransition*>(kevent.user.data1);
		if(tMod==this){
			printf("Matched\n");
			ScoreboardM* newScores=new ScoreboardM();
			newScores->SetTransition(tTrans);
			newScores->SetScores(activeRoom->GetScoreData());

			GMods::HookModule(newScores);
			GMods::SetFocus(newScores);
			GMods::RemoveHook(this);
			return;
		}
	}

	startButton->DoEvent(kevent);
	activeRoom->Event(kevent);
}

Pos2D GameModu::GetPanPos(){
	return Pos2D(panX, panY);
}

GameModu::GameModu(){
	mCode="game";
	Initialize();
}

GameModu::~GameModu(){
	Clean();
}

void GameModu::NewStage(RoomStage newStage){
	if(newStage==STAGE_BUILD){
		viewPY=100;
		startButton->SetVisible(true);
	} else {
		viewPY=0;
		startButton->SetVisible(false);
	}
	if(newStage==STAGE_SCORE){
		closeTrans=new Util::SteelTransition();
		closeTrans->SetLength(3000);
		Profiles::GetActiveProfile()->FinishedLevel(activeRoom->GetLevelName());
	}
	ResetPan();
}