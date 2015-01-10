#ifndef _NPC_HEADER_
#define _NPC_HEADER_

#include <deque>
#include "entities/entity.h"
#include "pathfind.h"
#include "SDL_thread.h"
#include "SDL_mutex.h"

struct SpawnQueueData{
	std::string eName;
	int quantity;
};

struct MutexPData{
	PFind::PathData* pData;
	SDL_mutex* lock;
};

enum SpawnDirection{
	SPAWN_LEFT=0,
	SPAWN_RIGHT,
	SPAWN_DOWN,
	SPAWN_UP,
	SPAWN_SELF
};

class ESpawnPoint : public EDrawable {
public:
	virtual void Draw();
	virtual void Event(SDL_Event &kevent);
	virtual void Think();
	virtual void SetPosition(int x, int y);
	virtual void StringParam(std::string str);
	virtual bool StateChange(RoomStage newStage);
	virtual WinCondition WinConditionState();
	ESpawnPoint();

	void CreateNextNPC();
	void CreatePath();
	void PathFound(MutexPData* mpData);
	void DestroyedEnt(EHeadQuarters* eHq);

	virtual bool IsOccupy(int x, int y);

	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
protected:
	int spriteNo;
	int spawnTime;
	bool canFind;
	unsigned int lastSpawn;
	std::deque<SpawnQueueData> spawnQueue;
	std::deque<Pos2D> toHq;
	SpawnDirection spawnDir;
	float xRel, yRel;
	EDrawable* targetEnt;

	SDL_Thread* pathFindThread;
	EHeadQuarters* desHq;
	EDrawable* newHq;
private:

};

struct DamageFloat{
	unsigned int TimeCreated;
	unsigned int Life;
	Colour col;
	char* dmgText;
	Pos2D pos;
};

class EBaseNPC : public EDrawable{
public:
	virtual void Draw();
	virtual void Event(SDL_Event &kevent);
	virtual void Think();
	virtual void SetPosition(int x, int y);
	//virtual void StringParam(std::string str);
	virtual bool StateChange(RoomStage newStage);
	virtual WinCondition WinConditionState();
	EBaseNPC();

	void SetSpawnedFrom(ESpawnPoint* spawner);
	ESpawnPoint* GetSpawnedFrom();
	void SetRelativePosition(float x, float y);
	void SetPath(std::deque<Pos2D> &path);
	void GotoNextCell();
	void SetAttacking(EDrawable* attack);
	void SetTargetEnt(EDrawable* target);
	EDrawable* GetTargetEnt();

	void Attack(int damage);

	virtual bool IsOccupy(int x, int y);
	static Sprite GetSprite();
	static const bool Purchasable;
	static const int Price;
	static const Pos2D pSize;
protected:
	int health;
	ESpawnPoint* spawnedFrom;
	std::deque<Pos2D> toHq;
	float xRel, yRel;
	float targetX, targetY;
	int spriteNo;
	unsigned int lastAttack;
	EDrawable* attacking;
	EDrawable* targetEnt;

	std::list<DamageFloat> damageFloats;
private:

};

#endif