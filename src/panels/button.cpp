#include "panels/panel.h"

float sign(float val){
	if(val>0) val=1; else val=0;
	return val;
}

void DButton::Draw(){
	Colour col;
	if(btnState==BUTTON_HOVER){
		col=Colour(100,100,100,255);
	} else if(btnState==BUTTON_DOWN){
		col=Colour(70,70,70,255);
	} else if(btnState==BUTTON_UP){
		col=Colour(110, 110, 110, 255);
	}
	GraphicsM::DrawRectangle(0, 0, PosW, PosH, col);
	int len=0;
	if(text!=NULL){
		len=strlen(text)*FontMan::GetFont("sm_text")->GetFontWidth();;
		float xp=((float)PosW*0.5)-((float)len*0.5);
		GraphicsM::DrawTText(xp, 2, FontMan::GetFont("sm_text"), text, 0, Colour(255,255,255,255));
	}
	if(btnIcon!=NULL){
		GraphicsM::DrawTexturedRectangle(2, 2, btnIconW, btnIconH, Colour(255,255,255,255), btnIcon);
	}
}

void DButton::Think(){

}

void DButton::PeformLayout(){

}

void DButton::Init(){
	btnState=BUTTON_UP;
	btnIcon=NULL;
	text=NULL;
	action=NULL;
	btnIconW=20;
	btnIconH=20;
	dataPointer=NULL;
}

void DButton::Clean(){

}

void DButton::OnClick(int x, int y, MouseAction mouseact){
	if(mouseact==MOUSE_UP){
		btnState=BUTTON_HOVER;
		if(action!=NULL){
			action(this, dataPointer);
		}
	} else if(mouseact==MOUSE_DOWN){
		btnState=BUTTON_DOWN;
	}
}

void DButton::SetText(char *stext){
	text=stext;
}

void DButton::SetAction(void (*saction)(DPanel*, void*)){
	action=saction;
}

void DButton::SetDataPointer(void* data){
	dataPointer=data;
}

DButton::DButton(int x, int y, int w) : DPanel(x, y, w, 25){
	Init();
}

void DButton::CursorEnter(){
	btnState=BUTTON_HOVER;
}

void DButton::CursorLeave(){
	btnState=BUTTON_UP;
}

void DButton::SetIcon(Texture *icon){
	btnIcon=icon;
}

void DButton::SetIconSize(int w, int h){
	btnIconW=w;
	btnIconH=h;
}