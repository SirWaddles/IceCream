#include "util/particles.h"
#include "util/timem.h"
#include "SDL.h"
#include <cmath>
#define PI 3.14159265

using namespace std;

namespace Util{

	Particle::Particle(){
		TimeCreated=SDL_GetTicks();
		pX=0.0;
		pY=0.0;
		pVelocity=10.0;
		pSize=2.0;
		pIntSize=2.0;
		pDiffSize=0.0;
		pRotation=0.0;
		pAlpha=255.0;

		pLife=1000;
		pVelDecay=0.0;
		pAlphaDecay=0.0;
		pSpinDecay=0.0;
	}

	void Particle::SetPosition(Pos2D pos){
		pX=pos.x;
		pY=pos.y;
	}

	void Particle::SetVelocity(float vel){
		pVelocity=vel;
	}

	void Particle::SetDirection(int dir){
		pDirection=float(dir%360)*(PI/180.0);
	}

	void Particle::SetSize(float size){
		pSize=size;
		pIntSize=size;
	}

	void Particle::SetRotation(float angle){
		pRotation=angle;
	}

	void Particle::SetSpin(float spin){
		pSpin=spin;
	}

	void Particle::SetAlpha(float alpha){
		pAlpha=alpha;
	}

	void Particle::SetColour(Colour col){
		pCol=col;
	}

	void Particle::SetLife(int life){
		pLife=life;
	}

	void Particle::SetVelDecay(float decay){
		pVelDecay=decay;
	}

	void Particle::SetAlphaDecay(float decay){
		//pAlphaDecay=1.0/decay;
		pAlphaDecay=decay;
	}

	void Particle::SetSpinDecay(float decay){
		pSpinDecay=1.0/decay;
	}

	void Particle::SetTexture(Texture* tex){
		particleTexture=tex;
	}

	void Particle::SetPEndSize(float size){
		pDiffSize=pIntSize-size;
	}

	void Particle::Think(){
		int Now=SDL_GetTicks();
		float stage=float(Now-TimeCreated)/pLife;
		float dX, dY;
		dX=sin(pDirection)*pVelocity*FrameTime();
		dY=cos(pDirection)*pVelocity*FrameTime();
		//printf("dX: %f\n", dX);
		pX+=dX;
		pY+=dY;
		pRotation+=pSpin*FrameTime();

		//pVelocity=pVelocity*(pVelDecay*(1-abs(FrameTime())));
		pVelocity-=(pVelDecay*FrameTime());
		pAlpha-=(pAlphaDecay*FrameTime());
		pSpin-=(pSpinDecay*FrameTime());
		pSize=pIntSize-(stage*pDiffSize);
	}

	void Particle::Draw(){
		pCol.a=pAlpha;
		GraphicsM::DrawTexturedRectangle(pX, pY, pSize, pSize, pCol, particleTexture, pRotation*(180/PI));
	}

	ParticleSystem::ParticleSystem(){

	}

	ParticleSystem::~ParticleSystem(){
		list<Particle*>::iterator it;
		for(it=particles.begin(); it!=particles.end(); it++){
			delete (*it);
		}
		particles.clear();
	}

	void ParticleSystem::AddParticle(Particle* part){
		particles.push_back(part);
	}

	void ParticleSystem::Think(){
		list<Particle*>::iterator it=particles.begin();
		int Now=SDL_GetTicks();
		while(it!=particles.end()){
			unsigned int endTime=(*it)->TimeCreated+(*it)->pLife;
			if(Now>endTime){
				delete (*it);
				particles.erase(it++);
				continue;
			}
			(*it)->Think();
			++it;
		}
	}

	void ParticleSystem::Draw(){
		list<Particle*>::iterator it;
		for(it=particles.begin(); it!=particles.end(); it++){
			(*it)->Draw();
		}
	}

}