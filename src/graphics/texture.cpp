#include "graphics/texture.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include <cmath>
#include <cassert>

int isPowerOfTwo (unsigned int x){
	return ((x != 0) && !(x & (x - 1)));
}

Texture::Texture(char *path){
	SDL_Surface *tex=IMG_Load(path);
	assert(tex!=NULL);
	tW=tex->w;
	tH=tex->h;
	GLuint textureLoc;
	GLenum texture_format;
	GLint nOfColors;
	if(!isPowerOfTwo(tex->w) || !isPowerOfTwo(tex->h)){
		return;
	}
	nOfColors=tex->format->BytesPerPixel;
	if(nOfColors==4){
		texture_format=GL_RGBA;
		alpha=true;
	} else if(nOfColors==3){
		texture_format=GL_RGB;
		alpha=false;
	} else {
		return;
	}

	glGenTextures(1, &textureLoc);
	glBindTexture(GL_TEXTURE_2D, textureLoc);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, tex->w, tex->h, 0, texture_format, GL_UNSIGNED_BYTE, tex->pixels);
	texLocate=textureLoc;
	SDL_FreeSurface(tex);
}

Texture::~Texture(){
	GLuint textureLoc=texLocate;
	glDeleteTextures(1, &textureLoc);
}

unsigned int Texture::GetTexName(){
	return texLocate;
}

bool Texture::IsAlphaEnabled(){
	return alpha;
}

int Texture::GetWidth(){
	return tW;
}

int Texture::GetHeight(){
	return tH;
}

SpriteSheet::SpriteSheet(char *path, int cW, int cH) : sprImg(path){
	//assert(isPowerOfTwo(cW) && isPowerOfTwo(cH));
	cellW=cW;
	cellH=cH;
	perRow=(float)sprImg.GetWidth()/(float)cW;
	rows=(float)sprImg.GetHeight()/(float)cH;
	cells=rows*perRow;

	/*printf("Creating sheet: %s\n", path);
	printf("CW: %d CH: %d\n", cW, cH);
	printf("IW: %d IH: %d\n", sprImg.GetWidth(), sprImg.GetHeight());
	printf("FW: %f FH: %f\n", GetCellW(), GetCellH());
	printf("5X: %f 5Y: %f\n", GetCellX(5), GetCellY(5));*/
}

Texture *SpriteSheet::GetTexture(){
	return &sprImg;
}

float SpriteSheet::GetCellX(int cell){
	if(cell>cells) return -1.0;
	int cellX=((cell-1)%(int)floor(perRow));
	return ((float)cellX)/(perRow);
}

float SpriteSheet::GetCellY(int cell){
	if(cell>cells) return -1.0;
	int cellY=((cell-1)/perRow);
	return ((float)cellY)/(rows);
}

float SpriteSheet::GetCellW(){
	return ((float)cellW)/((float)sprImg.GetWidth());
}

float SpriteSheet::GetCellH(){
	return ((float)cellH)/((float)sprImg.GetHeight());
}

int SpriteSheet::GetCellWi(){
	return cellW;
}

int SpriteSheet::GetCellHi(){
	return cellH;
}