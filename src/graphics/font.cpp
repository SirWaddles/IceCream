#include "graphics/font.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <map>
#include <string>

using namespace std;

namespace FontMan {
	map<string, TFont*> GFonts;
	void BuildFont(char *name, char *path, int size){
		GFonts[name]=new TFont(path, size);
	}

	void ClearFont(const char *name){
		map<string, TFont*>::iterator it=GFonts.find(name);
		if(it!=GFonts.end()){
			delete it->second;
			GFonts.erase(it);
		}
	}

	void ClearAllFonts(){
		map<string, TFont*>::iterator it;
		for(it=GFonts.begin(); it!=GFonts.end(); it++){
			const char *p;
			p=(*it).first.c_str();
			ClearFont(p);
		}
	}

	TFont *GetFont(char *name){
		map<string, TFont*>::iterator it=GFonts.find(name);
		if(it!=GFonts.end()){
			return GFonts[name];
		}
		return NULL;
	}
}

TFont::TFont(char *path, int size) : fontImg(path){
	float cx, cy;
	GLuint base=glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, fontImg.GetTexName());
	for(int loop=0;loop<256;loop++){
		cx=float(loop%16)/16.0f;
		cy=(float(loop/16)/16.0f);

		float mul=0.625;
		if(loop==73){
			mul=0.2;
		}
		glNewList(base+loop, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(cx,cy);
		glVertex2i(0,0);
		glTexCoord2f(cx+0.0625f,cy);
		glVertex2i(size,0);
		glTexCoord2f(cx+0.0625f,cy+0.0625f);
		glVertex2i(size, size);
		glTexCoord2f(cx,cy+0.0625f);
		glVertex2i(0,size);	
		glEnd();
		glTranslated(size*mul, 0, 0);
		glEndList();
	}
	fontLoc=base;
	fontWidth=((float)size)*0.625;
}

TFont::~TFont(){
	GLuint base=fontLoc;
	glDeleteLists(base, 256);
}

unsigned int TFont::GetFontName(){
	return fontLoc;
}

unsigned int TFont::GetTexName(){
	return fontImg.GetTexName();
}

unsigned int TFont::GetFontWidth(){
	return fontWidth;
}

bool TFont::GetTexAlpha(){
	return fontImg.IsAlphaEnabled();
}