#include "util/timem.h"
#include "SDL.h"

namespace Util{
	unsigned int lastTime=0;
	float frameTime=0;

	void UpdateFrameTime(){
		unsigned int newTime=SDL_GetTicks();
		frameTime=(float(newTime)-float(lastTime))/1000.0;
		lastTime=newTime;
	}

	float FrameTime(){
		return frameTime;
	}

	int Sign(int val){
		if(val>0){
			return 1;
		}
		if(val<0){
			 return -1;
		}
		return 0;
	}

	float Sign(float val){
		if(val>0.0){
			return 1.0;
		}
		if(val<0.0){
			 return -1.0;
		}
		return 0.0;
	}
}