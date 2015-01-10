#ifndef _ENTITY_HEADER_
#define _ENTITY_HEADER_

#include "SDL.h"
#include "graphics/texture.h"
#include "util/pos2d.h"
#include "util/particles.h"
#include <string>
#define TILE_SIZE_X 64
#define TILE_SIZE_Y 45

class TRoom;



enum RoomStage{
	STAGE_BUILD=1,
	STAGE_GAME,
	STAGE_SCORE
};

enum WinCondition{
	WIN_WIN=1,
	WIN_STAY,
	WIN_LOSE,
	WIN_NULL
};

class EDrawable {
public:
	virtual void Draw();
	virtual void Event(SDL_Event &kevent);
	virtual void Think();
	virtual void StringParam(std::string str);
	virtual bool StateChange(RoomStage newStage);
	virtual WinCondition WinConditionState();

	virtual void OnSelect();
	virtual void OnDeselect();

	EDrawable();

	float GetXPos();
	float GetYPos();
	int GetDrawOrder();
	virtual void SetPosition(int x, int y);
	//int GetXCell();
	//int GetYCell();
	virtual bool IsOccupy(int x, int y);

	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
	
	TRoom* parentRoom;
	char* className;
	bool Moveable;
private:

protected:
	int xCell, yCell;
	int DrawOrder;
};

class EBaseNPC;

class EHeadQuarters : public EDrawable {
public:
	virtual void Draw();
	virtual void Event(SDL_Event &kevent);
	virtual void Think();
	virtual WinCondition WinConditionState();
	EHeadQuarters();

	virtual bool IsOccupy(int x, int y);
	void Attack(int damage, EBaseNPC* attacker);

	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
	
protected:
	int spriteNo;
	int Health;
private:

};

class ECannonBall : public EDrawable {
public:
	virtual void Draw();
	virtual void Think();
	virtual bool IsOccupy(int x, int y);

	ECannonBall();

	void SetAttackPos(Pos2D attack);
	void SetEndPoint(unsigned int end);
	void SetOffset(float offset);
	void SetDistance(float distance);
	void SetInitialPos(Pos2D pos);

	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
protected:
	Pos2D cannonAttackPos;
	unsigned int lastAttack;
	unsigned int endPoint;
	float cannonOffset;
	float cannonDistance;
	Pos2D cannonPos;
	Pos2D initialPos;

	bool landed;
	Util::ParticleSystem partSys;
private:
};

class ECannonTower : public EDrawable {
public:
	virtual void Draw();
	virtual void Event(SDL_Event &kevent);
	virtual void Think();
	virtual bool IsOccupy(int x, int y);
	ECannonTower();

	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
protected:
	Pos2D attackPos;
	bool attacking;
	unsigned int lastAttack;
	ECannonBall* activeCannonBall;
private:


};



#endif