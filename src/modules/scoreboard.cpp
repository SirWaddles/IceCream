#include "scoreboard.h"
#include "graphics/window.h"
#include "modules/game.h"

using namespace GraphicsM;

void NextLevelBtnF(DPanel* self, void* data){
	ScoreboardM* score=static_cast<ScoreboardM*>(data);
	GameModu* newGame= new GameModu();
	GMods::HookModule(newGame);
	GMods::SetFocus(newGame);
	GMods::RemoveHook(score);
}

void MainMenuBtnF(DPanel* self, void* data){
	ScoreboardM* score=static_cast<ScoreboardM*>(data);
	GMods::SetFocus(GMods::GetModule("menu"));
	GMods::RemoveHook(score);
}

void ScoreboardM::Initialize(){
	openTrans=NULL;
	StartTime=SDL_GetTicks();
	cNpcs=new char[100];
	strcpy(cNpcs, "NPCs Killed: ");
	cDamage=new char[100];
	strcpy(cDamage, "Damage Dealt: ");
	cMoney=new char[100];
	strcpy(cMoney, "Money Earned: ");


	//Panels
	mainPanel=new DFrame((ScrW()/2.0)-300, (ScrH()/2.0)-200, 600, 400);
	mainPanel->ShowCloseButton(false);
	mainPanel->SetDraggable(false);
	mainPanel->SetText("The Wrap-up");

	DButton* nextLevelBtn=new DButton(20, 200, 150);
	nextLevelBtn->SetText("Next Level");
	nextLevelBtn->SetAction(NextLevelBtnF);
	nextLevelBtn->SetDataPointer(this);
	mainPanel->AddChild(nextLevelBtn);

	DButton* mainMenuBtn=new DButton(190, 200, 150);
	mainMenuBtn->SetText("Main Menu");
	mainMenuBtn->SetAction(MainMenuBtnF);
	mainMenuBtn->SetDataPointer(this);
	mainPanel->AddChild(mainMenuBtn);
}

void ScoreboardM::Clean(){
	delete cNpcs;
	delete cDamage;
	delete cMoney;
}

float aclamp(float v){
	if(v>1.0){
		v=1.0;
	}
	if(v<0.0){
		v=0.0;
	}
	return v;
}

void ScoreboardM::Draw(){
	GraphicsM::ClearWindow(Colour(0,0,0));
	int Now=SDL_GetTicks();
	int Since=Now-StartTime;

	float alph1=aclamp(float(Since-5000)/3000.0);
	float alph2=aclamp(float(Since-6000)/3000.0);
	float alph3=aclamp(float(Since-7000)/3000.0);

	mainPanel->DoDraw();

	GraphicsM::ResetOrigin(mainPanel->GetX(), mainPanel->GetY());

	GraphicsM::DrawTText(20, 60, FontMan::GetFont("sm_text"), cNpcs, 0, Colour(255,255,255,alph1*255.0));
	GraphicsM::DrawTText(20, 80, FontMan::GetFont("sm_text"), cDamage, 0, Colour(255,255,255,alph2*255.0));
	GraphicsM::DrawTText(20, 100, FontMan::GetFont("sm_text"), cMoney, 0, Colour(255,255,255,alph3*255.0));

	GraphicsM::ResetOrigin(-mainPanel->GetX(), -mainPanel->GetY());

	if(openTrans){
		openTrans->Draw();
	}
}

void ScoreboardM::Think(){
	if(openTrans){
		openTrans->Think();
		if(openTrans->Complete()){
			delete openTrans;
			openTrans=NULL;
		}
	}
	mainPanel->TopThink();
}

void ScoreboardM::BGThink(){

}

void ScoreboardM::Event(SDL_Event &kevent){
	mainPanel->DoEvent(kevent);
}

ScoreboardM::ScoreboardM(){
	Initialize();
}

ScoreboardM::~ScoreboardM(){

}

void ScoreboardM::SetTransition(Util::SteelTransition* nTrans){
	openTrans=nTrans;
}

void ScoreboardM::SetScores(RoomScoreData sData){
	scoreData=sData;
	char cBuf[100];
	strcpy(cNpcs, "NPCs Killed: ");
	itoa(sData.NPCsKilled, cBuf, 10);
	strcat(cNpcs, cBuf);
	strcpy(cDamage, "Damage Dealt: ");
	itoa(sData.DamageOnEnemy, cBuf, 10);
	strcat(cDamage, cBuf);
	strcpy(cMoney, "Money Earned: ");
	itoa(sData.Money, cBuf, 10);
	strcat(cMoney, cBuf);
}