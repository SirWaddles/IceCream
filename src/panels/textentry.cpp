#include "panels/panel.h"
#include <cmath>

DTextEntry::DTextEntry(int x, int y, int w) : DPanel(x, y, w, 25){
	text=NULL;
	SetWidth(w);
	Init();
}

void DTextEntry::Draw(){
	GraphicsM::DrawRectangle(-1, -1, PosW+2, PosH+2, Colour(0,0,0,255));
	GraphicsM::DrawRectangle(0, 0, PosW, PosH, Colour(255,255,255,255));
	int fontWidth=FontMan::GetFont("sm_text")->GetFontWidth();
	if(GetFocus()==this){
		int x=(strlen(text)*fontWidth)+2;
		GraphicsM::DrawTLine(x, 2, x, PosH-2, Colour(0,0,0,255));
	}
	if(text!=NULL){
		GraphicsM::DrawTText(0, 0, FontMan::GetFont("sm_text"), text, 0, Colour(0,0,0,255));
	}
}

void DTextEntry::Think(){

}

void DTextEntry::PeformLayout(){

}

void DTextEntry::Init(){
	SDL_EnableUNICODE(1);
}

void DTextEntry::Clean(){

}

void DTextEntry::OnClick(int x, int y, MouseAction mouseact){

}

void DTextEntry::CursorEnter(){

}

void DTextEntry::CursorLeave(){

}

void DTextEntry::Event(SDL_Event &kevent){
	if(kevent.type==SDL_KEYDOWN){
		int len=strlen(text);
		if(kevent.key.keysym.sym==SDLK_BACKSPACE){
			if(len<=0) return;
			text[len-1]=0;
		} else {
			if(len>=maxLetters) return;
			char ch;
			if((kevent.key.keysym.unicode & 0xFF80)==0){
				ch=kevent.key.keysym.unicode &0x7F;
				printf("Char copied: %c\n", ch);
				strncat(text, &ch, 1);
			}
		}
	}
}

void DTextEntry::SetWidth(int w){
	PosW=w;
	maxLetters=floor((float)w/15.0);
	char *pText=new char[maxLetters];
	if(text!=NULL){
		strncpy(pText, text, maxLetters);
		delete[] text;
	} else {
		strcpy(pText, "");
	}
	text=pText;
}

char* DTextEntry::GetText(){
	return text;
}