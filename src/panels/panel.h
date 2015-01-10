#ifndef _PANEL_HEADER_
#define _PANEL_HEADER_

#include <vector>
#include "SDL.h"
#include "graphics/window.h"

enum MouseAction{
	MOUSE_UP=0,
	MOUSE_DOWN
};

class DPanel {
public:
	DPanel(int x, int y, int w, int h);
	DPanel(int x, int y, int w, int h, DPanel *parent);
	DPanel();
	virtual ~DPanel();

	void TopThink();
	void DoDraw();
	void DoThink();
	void DoEvent(SDL_Event &kevent);
	void DoClick(MouseAction mouseact);
	DPanel *GetFocus();
	void SetFocus(DPanel *focus);
	DPanel *GetObject(int *x, int *y);
	bool IsHover();
	bool IsHover(int x, int y);
	void AddChild(DPanel *child);
	void SetParent(DPanel *parent);
	DPanel *GetParent();
	DPanel *GetTopParent();
	void SetVisible(bool visible);
	bool GetVisible();

	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	void DPanel::SetX(int x);
	void DPanel::SetY(int y);
	void DPanel::SetWidth(int w);
	void DPanel::SetHeight(int h);

	virtual void Draw();
	virtual void Think();
	virtual void PeformLayout();
	virtual void Init();
	virtual void Clean();
	virtual void OnClick(int x, int y, MouseAction mouseact);
	virtual void CursorEnter();
	virtual void CursorLeave();
	virtual void Event(SDL_Event &kevent);
	virtual void DrawOver();

protected:
	bool isVisible;
	int PosX, PosY;
	int PosW, PosH;
	DPanel* myParent;

private:
	std::vector<DPanel*> pChildren;
	DPanel *pHover;
	DPanel *pFocus;
};

enum ButtonState{
	BUTTON_DOWN=0,
	BUTTON_UP,
	BUTTON_HOVER
};

enum Alignment{
	ALIGN_LEFT=0,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

class DButton : public DPanel {
public:
	DButton(int x, int y, int w);
	virtual void Draw();
	virtual void Think();
	virtual void PeformLayout();
	virtual void Init();
	virtual void Clean();
	virtual void OnClick(int x, int y, MouseAction mouseact);
	virtual void CursorEnter();
	virtual void CursorLeave();

	void SetText(char *stext);
	void SetAction(void (*saction)(DPanel*, void*));
	void SetDataPointer(void* data);
	void SetIcon(Texture *icon);
	void SetIconSize(int w, int h);
protected:
	void (*action)(DPanel*, void*);
	void* dataPointer;
	char *text;
	ButtonState btnState;
	Texture *btnIcon;
	int btnIconW;
	int btnIconH;
};

class DFrame : public DPanel {
public:
	DFrame(int x, int y, int w, int h);
	
	virtual void Draw();
	virtual void Think();
	virtual void PeformLayout();
	virtual void Init();
	virtual void Clean();
	virtual void OnClick(int x, int y, MouseAction mouseact);
	virtual void CursorEnter();
	virtual void CursorLeave();

	void SetDraggable(bool drag);
	void SetText(char *text);
	void SetTextAlignment(Alignment align);
	void ShowCloseButton(bool show);
protected:
	bool dragging;
	bool IsDraggable;
	int dragClickX, dragClickY;
	int dragPosX, dragPosY;
	char *tText;
	Alignment tAlign;
private:
	DButton *closeButton;
};

class DTextEntry : public DPanel {
public:
	DTextEntry(int x, int y, int w);

	virtual void Draw();
	virtual void Think();
	virtual void PeformLayout();
	virtual void Init();
	virtual void Clean();
	virtual void OnClick(int x, int y, MouseAction mouseact);
	virtual void CursorEnter();
	virtual void CursorLeave();
	virtual void Event(SDL_Event &kevent);

	void SetWidth(int w);
	char* GetText();
protected:
	char *text;
	int maxLetters;
private:

};

class DRollout : public DPanel {
public:
	DRollout(int x, int y, int w, int h);

	virtual void Draw();
	virtual void Think();
	virtual void PeformLayout();
	virtual void Init();
	virtual void Clean();
	virtual void OnClick(int x, int y, MouseAction mouseact);
	virtual void CursorEnter();
	virtual void CursorLeave();
	virtual void Event(SDL_Event &kevent);
	virtual void DrawOver();

	void SetText(char *text);
protected:
	char *text;
	bool open;
	bool transition;
	int ToHeight;
	int SmoothHeight;
	double valHeight;
	int lastOpen;
private:

};

class DBGPanel : public DPanel {
public:
	virtual void Draw();
	DBGPanel(int x, int y, int w, int h, Colour col);
protected:
	Colour BGColour;
};

class DLabel : public DPanel {
public:
	virtual void Draw();
	DLabel(int x, int y, TFont* font, char* text);
	DLabel(int x, int y, TFont* font, char* text, Colour col);

	void SetAlignment(Alignment align);
	void SetText(char* text);
protected:
	char* lText;
	int tWidth;
	Colour lColour;
	TFont* lFont;
	Alignment lAlign;
};

#endif