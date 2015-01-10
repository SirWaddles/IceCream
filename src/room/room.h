#ifndef _ROOM_HEADER_
#define _ROOM_HEADER_

#include <list>
#include <string>
#include <map>
#include "entities/entity.h"
#include "util/leveldata.h"
#include "graphics/window.h"

class GameModu;

enum TileStyle{
	TILE_TILED=1,
	TILE_CIRCLES=2,
	TILE_GLOSS=3,
	TILE_WINDOWS=4
};

struct RoomScoreData{
	int NPCsKilled;
	int DamageOnEnemy;
	int DamageOnFriendly;
	int Money;
	RoomScoreData(){
		NPCsKilled=0;
		DamageOnEnemy=0;
		DamageOnFriendly=0;
		Money=0;
	}
};

class TRoom {
public:
	TRoom(int w, int h);
	~TRoom();

	void Draw();
	void Think();
	void SetTileStyle(TileStyle tileSty);
	int GetTotalSizeX();
	int GetTotalSizeY();
	int GetSizeX();
	int GetSizeY();
	Pos2D GetCell(int x, int y);
	bool IsOccupied(int x, int y, EDrawable* filter);
	bool IsOccupied(int x, int y);
	bool TRoom::IsOccupied(Pos2D pos, Pos2D size, EDrawable* filter);
	EDrawable* GetEntity(int x, int y);
	void AddDrawable(EDrawable *drawEnt);
	void RemoveDrawable(EDrawable *drawEnt, bool safe=false);
	void Event(SDL_Event &kevent);
	void CheckWin();
	EDrawable* GetSelectedEnt();

	void RecalculateCounts();
	void SetupLevel(Util::LevelData* lData);
	const char* GetLevelName();
	void ClearAllEnts();
	int GetCount(const std::string eName);
	int GetLimit(const std::string eName);

	void SetGameModu(GameModu* set);
	void SetStage(RoomStage setStage);
	void DrawBorder(Pos2D cell);
	void DrawBorder(Pos2D cell, Colour col);
	Pos2D GetHoverCell();
	Pos2D GetMouseCell(Pos2D mPos);
	RoomStage GetStage();
	GameModu* GetGameModu();

	void AddNPCKilled(int add);
	void AddDamageToEnemy(int add);
	void AddDamageToFriendly(int add);
	void AddMoneyGained(int add);
	RoomScoreData GetScoreData();

	std::list<EDrawable*> Drawables;
private:
	std::list<EDrawable*> toDelete;
	EDrawable* selectedEnt;
	int sizeW, sizeH;
	TileStyle spriteNo;
	GameModu* parentMod;
	RoomStage rmStage;
	Util::LevelData* levelData;
	std::map<std::string, int> entCounts;

	bool transitioning;
	unsigned int tranStart;
	Pos2D centerCell;

	RoomScoreData roomScore;
};


#endif