#include "panels/panel.h"

void DPanel::DoDraw(){
	if(!isVisible){
		return;
	}
	std::vector<DPanel*>::iterator it;
	GraphicsM::ResetOrigin(GetX(), GetY());
	Draw();
	for(it=pChildren.begin(); it<pChildren.end(); it++){
		(*it)->DoDraw();
	}
	DrawOver();
	GraphicsM::ResetOrigin(GetX()*-1, GetY()*-1);
}

void DPanel::TopThink(){
	if(!isVisible){
		return;
	}
	int x, y;
	SDL_GetMouseState(&x, &y);
	DPanel *nHover=GetObject(&x,&y);
	if(pHover!=nHover){
		if(pHover!=NULL){
			pHover->CursorLeave();
		}
		if(nHover!=NULL){
			nHover->CursorEnter();
		}
		pHover=nHover;
	}
	DoThink();
	//printf("%p\n", nHover);
}

void DPanel::DoThink(){
	if(!isVisible){
		return;
	}
	std::vector<DPanel*>::iterator it;
	Think();
	for(it=pChildren.begin(); it<pChildren.end(); it++){
		(*it)->DoThink();
	}
}

void DPanel::DoEvent(SDL_Event &kevent){
	if(!isVisible){
		return;
	}
	if(kevent.type==SDL_MOUSEBUTTONUP){
		DoClick(MOUSE_UP);
	} else if(kevent.type==SDL_MOUSEBUTTONDOWN){
		DoClick(MOUSE_DOWN);
	} else {
		if(pFocus!=NULL){
			pFocus->Event(kevent);
		} else {
			Event(kevent);
		}
	}
}

DPanel *DPanel::GetFocus(){
	return pFocus;
}

void DPanel::SetFocus(DPanel *focus){
	pFocus=focus;
}

//Magic
void DPanel::DoClick(MouseAction mouseact){
	if(!isVisible){
		return;
	}
	int x, y;
	SDL_GetMouseState(&x, &y);
	DPanel *pan=GetObject(&x, &y);
	if(pFocus!=NULL){
		pFocus->SetFocus(NULL);
	}
	pFocus=pan;
	if(pan==NULL){
		return;
	} else {
		pFocus->SetFocus(pan);
	}
	pan->OnClick(x, y, mouseact);
}

DPanel *DPanel::GetObject(int *x, int *y){
	if(!IsHover(*x, *y)){
		return NULL;
	}
	*x=*x-PosX;
	*y=*y-PosY;
	std::vector<DPanel*>::iterator it;
	for(it=pChildren.begin(); it<pChildren.end(); it++){
		DPanel *pan=(*it)->GetObject(x, y);
		if(pan!=NULL&&pan->GetVisible()){
			return pan;
		}
	}
	return this;
}

bool DPanel::IsHover(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	if(x>PosX && x<PosX+PosW && y>PosY && y<PosY+PosH){
		return true;
	} else {
		return false;
	}
}

bool DPanel::IsHover(int x, int y){
	if(x>PosX && x<PosX+PosW && y>PosY && y<PosY+PosH){
		return true;
	} else {
		return false;
	}
}

int DPanel::GetX(){
	return PosX;
}

int DPanel::GetY(){
	return PosY;
}

int DPanel::GetWidth(){
	return PosW;
}

int DPanel::GetHeight(){
	return PosH;
}

void DPanel::SetX(int x){
	PosX=x;
}

void DPanel::SetY(int y){
	PosY=y;
}

void DPanel::SetWidth(int w){
	PosW=w;
}

void DPanel::SetHeight(int h){
	PosH=h;
}

void DPanel::SetVisible(bool visible){
	isVisible=visible;
}

bool DPanel::GetVisible(){
	return isVisible;
}

DPanel::DPanel(int x, int y, int w, int h){
	PosX=x;
	PosY=y;
	PosW=w;
	PosH=h;
	Init();
	myParent=NULL;
	pHover=NULL;
	pFocus=NULL;
	isVisible=true;
}

DPanel::DPanel(int x, int y, int w, int h, DPanel *parent){
	DPanel(x, y, w, h);
	parent->AddChild(this);
	myParent=parent;
}

DPanel::DPanel(){
	DPanel(0, 0, 0, 0);
}

DPanel::~DPanel(){
	Clean();
	std::vector<DPanel*>::iterator it;
	for(it=pChildren.begin(); it<pChildren.end(); it++){
		delete (*it);
	}
}

void DPanel::AddChild(DPanel *child){
	pChildren.push_back(child);
	child->SetParent(this);
}

void DPanel::SetParent(DPanel *parent){
	myParent=parent;
}

DPanel *DPanel::GetParent(){
	return myParent;
}

DPanel *DPanel::GetTopParent(){
	DPanel *pan=GetParent();
	if(pan==NULL){
		return this;
	}
	DPanel *bufp=pan;
	while(bufp!=NULL){
		bufp=pan->GetParent();
		if(bufp!=NULL){
			pan=bufp;
		}
	}
	return pan;
}

void DPanel::Draw(){
	
}

void DPanel::Think(){

}

void DPanel::PeformLayout(){

}

void DPanel::Init(){

}

void DPanel::Clean(){

}

void DPanel::OnClick(int x, int y, MouseAction mouseact){
	
}

void DPanel::CursorEnter(){

}

void DPanel::CursorLeave(){

}

void DPanel::Event(SDL_Event &kevent){

}

void DPanel::DrawOver(){

}