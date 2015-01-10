#include "util/transitions.h"
#include "graphics/window.h"
#include "graphics/texture.h"

#include "SDL.h"

using namespace GraphicsM;

namespace Util{

	Texture* SteelDoor=NULL;

	SteelTransition::SteelTransition(){
		TimeStarted=SDL_GetTicks();
		tComplete=false;
		tReady=false;
		if(!SteelDoor){
			SteelDoor=new Texture("contents/steeldoor.png");
		}
	}

	void SteelTransition::SetLength(unsigned int len){
		Length=len;
	}

	void SteelTransition::Think(){
		unsigned int Now=SDL_GetTicks();
		tPerc=float(Now-TimeStarted)/float(Length);
		if(tPerc>2.0){
			tPerc=1.0-(tPerc-2.0);
		}
		if(tPerc<0.0){
			tPerc=0.0;
		}
		if(tPerc>1.0){
			tPerc=1.0;
			tReady=true;
		}
		if(tPerc>3.0){
			tComplete=true;
		}

	}

	void SteelTransition::Draw(){
		float parPerc=tPerc*tPerc;
		float xw=(float(ScrW())*0.5)*parPerc;
		int width=ScrH();
		//printf("Perc: %f\nParPerc: %f\n", tPerc, parPerc);
		//printf("xw: %f\n", xw);
		GraphicsM::DrawTexturedRectangle(xw-width, 0, width, width, Colour(255,255,255,255), SteelDoor);
		GraphicsM::DrawTHRectangle(ScrW()-xw, 0, width, width, Colour(255,255,255,255), SteelDoor, -1.0);
	}

	bool SteelTransition::Ready(){
		return tReady;
	}

	bool SteelTransition::Complete(){
		return tComplete;
	}


}