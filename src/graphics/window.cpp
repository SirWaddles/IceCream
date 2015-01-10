#include "graphics/window.h"
#include <cmath>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

namespace GraphicsM {

	SDL_Surface *screen;
	bool alphaOverride=false;
	float alphaMultiplier=1.0;
	void StartGraphics(){
		if((SDL_Init(SDL_INIT_VIDEO))==-1){
			return;
		}
		IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	}


	Texture *roundedCorner;

	void EndGraphics(){
		delete roundedCorner;

		IMG_Quit();
		SDL_FreeSurface(screen);
		SDL_Quit();
	}


	void CreateMainWindow(){
		SDL_WM_SetCaption("IceCreamGame", "Icon");
		ResizeWindow(800, 600);
		glEnable(GL_MULTISAMPLE);
		glDisable(GL_DEPTH_TEST);

		//Shh, you! It's part of the utility library!
		roundedCorner=new Texture("contents/rounded.png");
	}

	void ResizeWindow(int w, int h){

		screen=SDL_SetVideoMode(w, h, 32, SDL_OPENGL|SDL_RESIZABLE);
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4);

		printf("W: %d H: %d\n", w, h);
		printf("W: %d H: %d\n", screen->w, screen->h);

		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, 0, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	int ScrW(){
		return screen->w;
	}

	int ScrH(){
		return screen->h;
	}

	void WaitForFinish(){
		glFlush();
	}

	void AlphaOverride(bool toSet){
		alphaOverride=toSet;
	}

	void SetAlphaMultiplier(float alpha){
		alphaMultiplier=alpha;
	}

	void ClearWindow(Colour col){
		GLfloat r=(GLfloat)col.r/255.0f;
		GLfloat g=(GLfloat)col.g/255.0f;
		GLfloat b=(GLfloat)col.b/255.0f;
		glClearColor(r, g, b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DrawRectangle(int x, int y, int w, int h, Colour col){
		glDisable(GL_TEXTURE_2D);
		if(col.a<255||alphaOverride){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		glBegin(GL_QUADS);
		if(alphaOverride){
			col.a=col.a*alphaMultiplier;
		}
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glVertex2i(x, y);
		glVertex2i(x+w, y);
		glVertex2i(x+w, y+h);
		glVertex2i(x, y+h);
		glEnd();
	}

	void DrawSprite(int x, int y, int cell, SpriteSheet *sprSheet){
		glEnable(GL_TEXTURE_2D);
		if(sprSheet->GetTexture()->IsAlphaEnabled()){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		GLfloat cW=sprSheet->GetCellW();
		GLfloat cH=sprSheet->GetCellH();
		GLfloat cX=sprSheet->GetCellX(cell);
		GLfloat cY=sprSheet->GetCellY(cell);
		int w=sprSheet->GetCellWi();
		int h=sprSheet->GetCellHi();
		glBindTexture(GL_TEXTURE_2D, sprSheet->GetTexture()->GetTexName());
		glColor4f(1.0, 1.0, 1.0, 1.0);
		if(alphaOverride){
			glColor4f(1.0, 1.0, 1.0, alphaMultiplier);
		}
		glBegin(GL_QUADS);
			glTexCoord2d(cX, cY);
			glVertex2i(x, y);
			glTexCoord2d(cX+cW, cY);
			glVertex2i(x+w, y);
			glTexCoord2d(cX+cW, cY+cH);
			glVertex2i(x+w, y+h);
			glTexCoord2d(cX, cY+cH);
			glVertex2i(x, y+h);
		glEnd();
	}

	void DrawSprite(int x, int y, int cell, SpriteSheet *sprSheet, Colour col){
		glEnable(GL_TEXTURE_2D);
		if(sprSheet->GetTexture()->IsAlphaEnabled()||col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		GLfloat cW=sprSheet->GetCellW();
		GLfloat cH=sprSheet->GetCellH();
		GLfloat cX=sprSheet->GetCellX(cell);
		GLfloat cY=sprSheet->GetCellY(cell);
		int w=sprSheet->GetCellWi();
		int h=sprSheet->GetCellHi();
		glBindTexture(GL_TEXTURE_2D, sprSheet->GetTexture()->GetTexName());
		if(alphaOverride){
			col.a=col.a*alphaMultiplier;
		}
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glBegin(GL_QUADS);
			glTexCoord2d(cX, cY);
			glVertex2i(x, y);
			glTexCoord2d(cX+cW, cY);
			glVertex2i(x+w, y);
			glTexCoord2d(cX+cW, cY+cH);
			glVertex2i(x+w, y+h);
			glTexCoord2d(cX, cY+cH);
			glVertex2i(x, y+h);
		glEnd();
	}

	void DrawRoundedRectangle(int radius, int x, int y, int w, int h, Colour col){
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, roundedCorner->GetTexName());
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glBegin(GL_QUADS);
			glTexCoord2d(0.0, 0.0);
			glVertex2i(x, y);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(x, y+radius);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(x+radius, y+radius);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(x+radius, y);

			glTexCoord2d(0.0, 0.0);
			glVertex2i(x+w, y);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(x+w, y+radius);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(x+w-radius, y+radius);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(x+w-radius, y);

			glTexCoord2d(0.0, 0.0);
			glVertex2i(x, y+h);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(x, y+h-radius);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(x+radius, y+h-radius);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(x+radius, y+h);

			glTexCoord2d(0.0, 0.0);
			glVertex2i(x+w, y+h);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(x+w, y+h-radius);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(x+w-radius, y+h-radius);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(x+w-radius, y+h);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_BLEND);
		glBegin(GL_QUADS);
			glVertex2i(x+radius, y);
			glVertex2i(x+w-radius, y);
			glVertex2i(x+w-radius, y+radius);
			glVertex2i(x+radius, y+radius);

			glVertex2i(x+radius, y+h);
			glVertex2i(x+w-radius, y+h);
			glVertex2i(x+w-radius, y+h-radius);
			glVertex2i(x+radius, y+h-radius);

			glVertex2i(x, y+radius);
			glVertex2i(x+w, y+radius);
			glVertex2i(x+w, y+h-radius);
			glVertex2i(x, y+h-radius);
		glEnd();
	}

	void DrawTexturedRectangle(int x, int y, int w, int h, Colour col, Texture *tex){
		glEnable(GL_TEXTURE_2D);
		if(tex->IsAlphaEnabled()||col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindTexture(GL_TEXTURE_2D, (GLuint)tex->GetTexName());
		glBegin(GL_QUADS);
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glTexCoord2d(0.0, 0.0);
		glVertex2i(x, y);
		glTexCoord2d(1.0, 0.0);
		glVertex2i(x+w, y);
		glTexCoord2d(1.0, 1.0);
		glVertex2i(x+w, y+h);
		glTexCoord2d(0.0, 1.0);
		glVertex2i(x, y+h);
		glEnd();
	}

	void DrawTHRectangle(int x, int y, int w, int h, Colour col, Texture* tex, float perc){
		glEnable(GL_TEXTURE_2D);
		if(tex->IsAlphaEnabled()||col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindTexture(GL_TEXTURE_2D, (GLuint)tex->GetTexName());
		float aPerc=fabs(perc);
		if(perc>0){
			glBegin(GL_QUADS);
			glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
			glTexCoord2d(0.0, 0.0);
			glVertex2i(x, y);
			glTexCoord2d(perc, 0.0);
			glVertex2i(x+(float(w)*perc), y);
			glTexCoord2d(perc, 1.0);
			glVertex2i(x+(float(w)*perc), y+h);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(x, y+h);
			glEnd();
		} else {
			glBegin(GL_QUADS);
			glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(x, y);
			glTexCoord2d(1.0-aPerc, 0.0);
			glVertex2i(x+(float(w)*aPerc), y);
			glTexCoord2d(1.0-aPerc, 1.0);
			glVertex2i(x+(float(w)*aPerc), y+h);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(x, y+h);
			glEnd();
		}
	}

	void DrawTexturedRectangle(int x, int y, int w, int h, Colour col, Texture *tex, float angle){
		glPushMatrix();
		glTranslatef(x, y, 0);
		float halfx=(float)w*0.5;
		float halfy=(float)h*0.5;
		glTranslatef(halfx, halfy, 0);
		glRotatef(angle, 0, 0, 1);
		glTranslatef(-halfx, -halfy, 0);
		glEnable(GL_TEXTURE_2D);
		if(tex->IsAlphaEnabled()||col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindTexture(GL_TEXTURE_2D, (GLuint)tex->GetTexName());
		glBegin(GL_QUADS);
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glTexCoord2d(0.0, 0.0);
		glVertex2i(0, 0);
		glTexCoord2d(1.0, 0.0);
		glVertex2i(w, 0);
		glTexCoord2d(1.0, 1.0);
		glVertex2i(w, h);
		glTexCoord2d(0.0, 1.0);
		glVertex2i(0, h);
		glEnd();
		glPopMatrix();
	}

	void DrawTText(int x, int y, TFont *tfont, char *string, int set, Colour col){
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		if(tfont->GetTexAlpha()||col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		glBindTexture(GL_TEXTURE_2D, tfont->GetTexName());
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glTranslated(x, y, 0);
		glListBase(tfont->GetFontName()-32);
		glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
		glPopMatrix();
	}
	
	void ResetOrigin(int x, int y){
		glTranslatef(x, y, 0);
	}

	void ResetScale(float x, float y){
		glScalef(x, y, 1.0);
	}

	void DrawTLine(int x1, int y1, int x2, int y2, Colour col){
		if(col.a<255){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}
		glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0);
		glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glEnd();
	}

	void EnablePlane(int a, int b, int c, int d){
		double eq[4]={a, b, c, d};
		glClipPlane(GL_CLIP_PLANE1, eq);
		glEnable(GL_CLIP_PLANE1);
	}

	void EnablePlane(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3){
		double eq[4];
		eq[0] = (y1*(z2 - z3)) + (y2*(z3 - z1)) + (y3*(z1 - z2));
		eq[1] = (z1*(x2 - x3)) + (z2*(x3 - x1)) + (z3*(x1 - x2));
		eq[2] = (x1*(y2 - y3)) + (x2*(y3 - y1)) + (x3*(y1 - y2));
		eq[3] = -((x1*((y2*z3) - (y3*z2))) + (x2*((y3*z1) - (y1*z3))) + (x3*((y1*z2) - (y2*z1))));
	}

	void DisablePlane(){
		glDisable(GL_CLIP_PLANE1);
	}

}