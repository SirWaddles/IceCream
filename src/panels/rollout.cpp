#include "panels/panel.h"

void DRollout::Draw(){
	GraphicsM::DrawRectangle(0, 0, PosW, PosH, Colour(50, 50, 50));
	GraphicsM::DrawRectangle(2, 2, PosW-4, 21, Colour(100, 100, 100));
	int len=0;
	if(text!=NULL){
		len=strlen(text)*15;
		float xp=((float)PosW*0.5)-((float)len*0.5);
		GraphicsM::DrawTText(xp, 2, FontMan::GetFont("text"), text, 0, Colour(255,255,255,255));
	}
	GraphicsM::EnablePlane(0, -1, 0, PosH-2);
}

void DRollout::DrawOver(){
	GraphicsM::DisablePlane();
}

void DRollout::Think(){
	double diff=(valHeight-SmoothHeight)/10;
	if(abs(diff)<=0.1){
		valHeight=SmoothHeight;
	}
	valHeight=valHeight-diff;
	PosH=valHeight;
}

void DRollout::PeformLayout(){

}

void DRollout::Init(){

}

void DRollout::Clean(){

}

void DRollout::OnClick(int x, int y, MouseAction mouseact){
	if(y<25&&mouseact==MOUSE_UP){
		if(open){
			SmoothHeight=25;
		} else {
			SmoothHeight=ToHeight;
		}
		lastOpen=SDL_GetTicks();
		open=!open;
		transition=true;
	}
}

void DRollout::CursorEnter(){
	
}

void DRollout::CursorLeave(){

}

void DRollout::Event(SDL_Event &kevent){

}

DRollout::DRollout(int x, int y, int w, int h) : DPanel(x, y, w, 25){
	ToHeight=h;
	SmoothHeight=25;
	valHeight=25;
	open=false;
	transition=false;
	text=NULL;
	Init();
}

void DRollout::SetText(char *stext){
	text=stext;
}