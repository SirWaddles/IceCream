#include "panels/panel.h"

DBGPanel::DBGPanel(int x, int y, int w, int h, Colour col) : DPanel(x, y, w, h){
	BGColour=col;
}

void DBGPanel::Draw(){
	GraphicsM::DrawRoundedRectangle(4, 0, 0, PosW, PosH, BGColour);
	//printf("X: %d Y: %d W: %d H: %d\n", PosX, PosY, PosW, PosH);
}