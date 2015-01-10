#include "entities/manager.h"


using namespace std;

namespace Ents{

	map<string, IManEntry*> *entityListing=NULL;
	void DefineNewEntity(IManEntry* toAdd){
		if(!entityListing){
			entityListing=new map<string, IManEntry*>;
		}
		string eName=toAdd->GetType();
		int count = (*entityListing).count(eName);
		if(count!=0){
			throw -1;
		}
		(*entityListing)[eName]=toAdd;
	}

	map<string, IManEntry*>& GetEMap(){
		return *entityListing;
	}

	EDrawable* CreateEntity(char* eName){
		map<string, IManEntry*>::iterator it;
		it=GetEMap().find(eName);
		if(it==GetEMap().end()){
			return NULL;
		}
		EDrawable* newEnt=it->second->Create();
		return newEnt;
	}
	const int GetEntPrice(char* eName){
		map<string, IManEntry*>::iterator it;
		it=GetEMap().find(eName);
		if(it==GetEMap().end()){
			return 0;
		}
		return it->second->GetPrice();
	}

	const bool IsEntPurchasable(char* eName){
		map<string, IManEntry*>::iterator it;
		it=GetEMap().find(eName);
		if(it==GetEMap().end()){
			return false;
		}
		return it->second->IsPurchasable();
	}

	Sprite GetEntSprite(char* eName){
		map<string, IManEntry*>::iterator it;
		it=GetEMap().find(eName);
		if(it==GetEMap().end()){
			return Sprite();
		}
		return it->second->GetSprite();
	}

	const Pos2D GetEntSize(char* eName){
		map<string, IManEntry*>::iterator it;
		it=GetEMap().find(eName);
		if(it==GetEMap().end()){
			return Pos2D(0,0);
		}
		return it->second->GetSize();
	}
}