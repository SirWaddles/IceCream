#ifndef _WINDOWHEADER_
#define _WINDOWHEADER_

#include "graphics/font.h"
//#include "graphics/texture.h"

struct Colour{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	Colour(unsigned char red, unsigned char green, unsigned char blue):r(red), g(green), b(blue), a(255){};
	Colour():r(0), g(0), b(0), a(255){};
	Colour(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha):r(red), g(green), b(blue), a(alpha){};
};

namespace GraphicsM {
	void StartGraphics();
	void EndGraphics();
	void CreateMainWindow();
	void ResizeWindow(int width, int height);
	void ClearWindow(Colour col);
	void WaitForFinish();
	int ScrW();
	int ScrH();

	void DrawRectangle(int x, int y, int w, int h, Colour col);
	void DrawRoundedRectangle(int radius, int x, int y, int w, int h, Colour col);
	void DrawTexturedRectangle(int x, int y, int w, int h, Colour col, Texture *tex);
	void DrawTHRectangle(int x, int y, int w, int h, Colour col, Texture* tex, float perc);
	void DrawTexturedRectangle(int x, int y, int w, int h, Colour col, Texture *tex, float angle);
	void DrawTText(int x, int y, TFont *tfont, char *string, int set, Colour col);
	void ResetOrigin(int x, int y);
	void ResetScale(float x, float y);
	void DrawTLine(int x1, int y1, int x2, int y2, Colour col);
	void DrawSprite(int x, int y, int cell, SpriteSheet *sprSheet);
	void DrawSprite(int x, int y, int cell, SpriteSheet *sprSheet, Colour col);

	void AlphaOverride(bool toSet);
	void SetAlphaMultiplier(float alpha);

	void EnablePlane(int a, int b, int c, int d);
	void DisablePlane();
}

#endif