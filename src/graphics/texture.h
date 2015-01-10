#ifndef _TEXTURE_HEADER_
#define _TEXTURE_HEADER_

//#include "SDL.h"
//#include "SDL_opengl.h"

class Texture {
public:
	Texture(char *path);
	~Texture();

	unsigned int GetTexName();
	bool IsAlphaEnabled();
	int GetWidth();
	int GetHeight();
private:
	unsigned int texLocate;
	bool alpha;
	int tW, tH;

};

class SpriteSheet {
public:
	SpriteSheet(char *path, int cW, int cH);
	Texture* GetTexture();
	float GetCellX(int cell);
	float GetCellY(int cell);
	float GetCellW();
	float GetCellH();
	int GetCellWi();
	int GetCellHi();
private:
	Texture sprImg;
	int cellW, cellH;
	int imgW, imgH;
	float perRow;
	float rows;
	int cells;
};

struct Sprite{
	SpriteSheet* mSheet;
	int cellNo;
	Sprite(SpriteSheet* sheet=0, int cell=0){
		mSheet=sheet;
		cellNo=cell;
	}
};

#endif