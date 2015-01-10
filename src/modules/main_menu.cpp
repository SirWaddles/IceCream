#include "modules/main_menu.h"
#include "modules/game.h"
#include "panels/panel.h"
#include <cmath>
#include "profiles.h"
#define PI 3.14159265

using namespace GraphicsM;
using namespace Profiles;

void ForceQuit();

DFrame* menuFrame;
DFrame* factionFrame;

Texture* titleTex=NULL;

Texture* iceCreamLogo=NULL;
Texture* fireBallLogo=NULL;

Faction selectFaction=FACTION_UNASSIGNED;
DTextEntry* ProfileName=NULL;
DButton* NewGamebtn=NULL;
char* cProfileName=NULL;

void NewGame(){
	GModule* gameModule=GMods::GetModule("game");
	if(!gameModule){
		GameModu* gameModule=new GameModu();
		GMods::HookModule(gameModule);
		GMods::SetFocus(gameModule);
	} else {
		GMods::SetFocus(gameModule);
	}
}

void NewGameBtn(DPanel* button, void* data){
	MainMenu* menu=static_cast<MainMenu*>(data);
	menu->transAct=NewGame;
	menu->StartTransition();
}

void MainMenu::StartTransition(){
	transitioning=true;
	tranStart=SDL_GetTicks();
}

void CreateProfile(){
	if(strlen(ProfileName->GetText())>0){
		cProfileName=ProfileName->GetText();
		AddNewProfile(cProfileName);
		SelectProfile(cProfileName);
		GetActiveProfile()->SetFaction(selectFaction);
		if(selectFaction==FACTION_FIRE_BALL){
			GetActiveProfile()->SetCampaign("fireball");
		}
		if(selectFaction==FACTION_ICE_CREAM){
			GetActiveProfile()->SetCampaign("icecream");
		}
		factionFrame->SetVisible(false);
		menuFrame->SetVisible(true);
	}
}

void SelectFireball(DPanel* button, void* data){
	selectFaction=FACTION_FIRE_BALL;
	CreateProfile();
}

void SelectIceCream(DPanel* button, void* data){
	selectFaction=FACTION_ICE_CREAM;
	CreateProfile();
}

void QuitGame(DPanel* button, void* data){
	ForceQuit();
}

void MainMenu::Initialize(){
	transitioning=false;
	titleTex=new Texture("contents/title.png");
	FontMan::BuildFont("title", "contents/title1.png", 64);
	FontMan::BuildFont("text", "contents/text1.png", 24);
	FontMan::BuildFont("sm_text", "contents/sm_text.png", 16);
	menuFrame=new DFrame(ScrW()-240, 20, 200, ScrH()-40);
	menuFrame->SetText("Main Menu");
	menuFrame->SetDraggable(false);
	menuFrame->ShowCloseButton(false);

	DBGPanel* mSection1=new DBGPanel(10, 40, 180, 200, Colour(0, 0, 0, 100));
	menuFrame->AddChild(mSection1);

	NewGamebtn=new DButton(10, 10, 160);
	NewGamebtn->SetText("New Game");
	NewGamebtn->SetAction(NewGameBtn);
	NewGamebtn->SetDataPointer(this);
	mSection1->AddChild(NewGamebtn);

	DButton* QuitGamebtn=new DButton(10, 40, 160);
	QuitGamebtn->SetText("Quit Game");
	QuitGamebtn->SetAction(QuitGame);
	mSection1->AddChild(QuitGamebtn);

	menuFrame->SetVisible(false);

	iceCreamLogo=new Texture("contents/ice.png");
	fireBallLogo=new Texture("contents/fire.png");

	factionFrame=new DFrame(100, ((float)ScrH()*0.5)-150.0, ScrW()-200, 300);
	factionFrame->SetText("Select your Faction");
	factionFrame->SetTextAlignment(ALIGN_CENTER);
	factionFrame->ShowCloseButton(false);

	int factionCenter=((float)factionFrame->GetWidth())*0.5;

	DButton* IceCreambtn=new DButton(factionCenter-140, 130, 132);
	IceCreambtn->SetHeight(132);
	IceCreambtn->SetIcon(iceCreamLogo);
	IceCreambtn->SetIconSize(128, 128);
	IceCreambtn->SetAction(SelectIceCream);
	factionFrame->AddChild(IceCreambtn);

	DButton* FireBallbtn=new DButton(factionCenter+8, 130, 132);
	FireBallbtn->SetHeight(132);
	FireBallbtn->SetIconSize(128, 128);
	FireBallbtn->SetIcon(fireBallLogo);
	FireBallbtn->SetAction(SelectFireball);
	factionFrame->AddChild(FireBallbtn);

	DLabel* FireBallText=new DLabel(factionCenter+70, 262, FontMan::GetFont("sm_text"), "Fireballs");
	FireBallText->SetAlignment(ALIGN_CENTER);
	factionFrame->AddChild(FireBallText);

	DLabel* IceCreamText=new DLabel(factionCenter-70, 262, FontMan::GetFont("sm_text"), "Ice-Cream");
	IceCreamText->SetAlignment(ALIGN_CENTER);
	factionFrame->AddChild(IceCreamText);

	DLabel* ProfileText=new DLabel(factionCenter-200, 45, FontMan::GetFont("sm_text"), "Please enter your name.");
	factionFrame->AddChild(ProfileText);

	DLabel* FactionSelectTxt=new DLabel(factionCenter-200, 100, FontMan::GetFont("sm_text"), "and select a faction.");
	factionFrame->AddChild(FactionSelectTxt);

	ProfileName=new DTextEntry(factionCenter-200, 70, 400);
	factionFrame->AddChild(ProfileName);
}

void MainMenu::Clean(){
	delete menuFrame;
	delete factionFrame;
	delete titleTex;

	delete iceCreamLogo;
	delete fireBallLogo;
}

void MainMenu::Draw(){
	GraphicsM::ClearWindow(Colour(20, 20, 30));
	int titleX = ((float)ScrW()*0.5)-256-120;
	float angle=((float)SDL_GetTicks()*0.002)/PI;
	angle=sin(angle);
	//DrawTexturedRectangle(titleX, 50, 512, 256, Colour(255,255,255,255), titleTex, angle*5);
	menuFrame->DoDraw();
	factionFrame->DoDraw();

	if(cProfileName!=NULL){
		char str[100];
		strcpy(str, "Welcome, ");
		strcat(str, cProfileName);
		GraphicsM::DrawTText(20, 20, FontMan::GetFont("sm_text"), str, 0, Colour(255,255,255,255));
	}
	if(transitioning){
		unsigned int Now=SDL_GetTicks();
		float tranStage=float(Now-tranStart)/1000.0;
		if(tranStage<=0.0) tranStage=0.0;
		if(tranStage>=1.0) tranStage=1.0;
		GraphicsM::DrawRectangle(-1, -1, ScrW(), ScrH(), Colour(0,0,0,floor(255.0*tranStage)));
		if(tranStage>=1.0){
			transAct();
			transitioning=false;
			return;
		}
	}
}

void MainMenu::Think(){
	menuFrame->TopThink();
	factionFrame->TopThink();
}

void MainMenu::BGThink(){

}

void MainMenu::Event(SDL_Event &kevent){
	if(kevent.type==SDL_VIDEORESIZE){
		menuFrame->SetX(ScrW()-240);
		menuFrame->SetHeight(ScrH()-40);
	}
	menuFrame->DoEvent(kevent);
	factionFrame->DoEvent(kevent);
}

void MainMenu::ChangeModule(int nIndex){
	if(GMods::GetModule("game")){
		NewGamebtn->SetText("Resume Game");
	}
}

MainMenu::~MainMenu(){
	Clean();
}

MainMenu::MainMenu(){
	Initialize();
	mCode="menu";
}