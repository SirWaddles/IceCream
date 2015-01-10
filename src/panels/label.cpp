#include "panels/panel.h"

DLabel::DLabel(int x, int y, TFont* font, char* text, Colour col): DPanel(x, y, 10, 10){
	int length=strlen(text);
	int fontWidth=font->GetFontWidth();
	int w=length*fontWidth;
	tWidth=w;
	SetHeight(fontWidth*2);
	SetWidth(w);

	lText=text;
	lFont=font;
	lColour=col;
	lAlign=ALIGN_LEFT;
}

DLabel::DLabel(int x, int y, TFont* font, char* text): DPanel(x, y, 10, 10){
	int length=strlen(text);
	int fontWidth=font->GetFontWidth();
	int w=length*fontWidth;
	tWidth=w;
	//SetHeight(fontWidth*2);
	//SetWidth(w);

	lText=text;
	lFont=font;
	lColour=Colour(255,255,255,255);
	lAlign=ALIGN_LEFT;
}

void DLabel::Draw(){
	//GraphicsM::DrawRectangle(0, 0, 10, 10, Colour(0,0,0,255));
	if(lAlign==ALIGN_LEFT){
		GraphicsM::DrawTText(0, 0, lFont, lText, 0, lColour);
	} else if(lAlign==ALIGN_CENTER){
		GraphicsM::DrawTText(((float)tWidth)*-0.5, 0, lFont, lText, 0, lColour);
	} else if(lAlign==ALIGN_RIGHT){
		GraphicsM::DrawTText(-tWidth, 0, lFont, lText, 0, lColour);
	}
}

void DLabel::SetAlignment(Alignment align){
	lAlign=align;
}

void DLabel::SetText(char* text){
	lText=text;
}