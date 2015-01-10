#include "panels/panel.h"
#include "SDL.h"

DFrame::DFrame(int x, int y, int w, int h): DPanel(x, y, w, h){
	Init();
	IsDraggable=true;
}

Texture *btnCloseTex=NULL;

void DFrame::Draw(){
	GraphicsM::DrawRoundedRectangle(4, 0, 0, PosW, PosH, Colour(90, 90, 90, 255));
	GraphicsM::DrawTLine(0, 30, PosW, 30, Colour(255,255,255,255));
	if(tText!=NULL){
		int chars=strlen(tText);
		int textWidth=chars*FontMan::GetFont("sm_text")->GetFontWidth();
		float xp=0.0;
		if(tAlign==ALIGN_LEFT){
			xp=5.0;
		} else if(tAlign==ALIGN_CENTER){
			xp=(PosW*0.5)-((float)textWidth*0.5);
		} else if(tAlign==ALIGN_RIGHT){
			xp=PosW-textWidth-5;
		}
		GraphicsM::DrawTText(xp, 2, FontMan::GetFont("sm_text"), tText, 0, Colour(255,255,255,255));
	}
}

void DFrame::Think(){
	if(dragging&&IsDraggable){
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		int diffX=mx-dragClickX;
		int diffY=my-dragClickY;
		PosX=dragPosX+diffX;
		PosY=dragPosY+diffY;
	}
}

void DFrame::PeformLayout(){

}

void CloseButtonFunc(DPanel *button, void* data){
	button->GetParent()->SetVisible(false);
}

void DFrame::Init(){
	if(btnCloseTex==NULL){
		btnCloseTex=new Texture("contents/close.png");
	}
	DButton *btnClose=new DButton(PosW-30, 3, 25);
	btnClose->SetAction(CloseButtonFunc);
	btnClose->SetIcon(btnCloseTex);
	closeButton=btnClose;
	AddChild(btnClose);
	dragging=false;
	tText=NULL;
	tAlign=ALIGN_LEFT;
}

void DFrame::Clean(){

}

void DFrame::OnClick(int x, int y, MouseAction mouseact){
	if(y<25 && IsDraggable){
		if(mouseact==MOUSE_DOWN){
			dragging=true;
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			dragClickX=mx;
			dragClickY=my;
			dragPosX=PosX;
			dragPosY=PosY;
		}
	}
	if(mouseact==MOUSE_UP&&IsDraggable){
		dragging=false;
	}
}

void DFrame::CursorEnter(){

}

void DFrame::CursorLeave(){

}

void DFrame::SetText(char *text){
	tText=text;
}

void DFrame::SetDraggable(bool drag){
	IsDraggable=drag;
}

void DFrame::SetTextAlignment(Alignment align){
	tAlign=align;
}

void DFrame::ShowCloseButton(bool show){
	closeButton->SetVisible(show);
}