#ifndef _PARTICLE_HEADER_
#define _PARTICLE_HEADER_

#include "graphics/texture.h"
#include "graphics/window.h"
#include "util/pos2d.h"
#include <list>

namespace Util{
	class Particle{
	public:
		Particle();
		void SetPosition(Pos2D pos);
		void SetVelocity(float vel);
		void SetDirection(int dir);
		void SetSize(float size);
		void SetRotation(float angle);
		void SetSpin(float spin);
		void SetAlpha(float alpha);
		void SetColour(Colour col);

		void SetLife(int life);
		void SetVelDecay(float decay);
		void SetAlphaDecay(float decay);
		void SetSpinDecay(float decay);
		void SetPEndSize(float size);

		void SetTexture(Texture* tex);

		void Think();
		void Draw();
		unsigned int TimeCreated;
		int pLife;
	private:
		float pX,pY;
		float pVelocity;
		float pDirection;
		float pSize;
		float pIntSize;
		float pRotation;
		float pSpin;
		float pAlpha;
		Colour pCol;

		float pVelDecay;
		float pAlphaDecay;
		float pSpinDecay;
		float pDiffSize;

		Texture* particleTexture;
	};

	class ParticleSystem{
	public:
		ParticleSystem();
		~ParticleSystem();
		void AddParticle(Particle* part);

		void Think();
		void Draw();
	private:
		std::list<Particle*> particles;
	};
}


#endif