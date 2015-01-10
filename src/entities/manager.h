#ifndef _EMANS_HEADER_
#define _EMANS_HEADER_

#include "entities/entity.h"
#include "graphics/texture.h"
#include <map>
#include <string>

using namespace std;

namespace Ents{

	class IManEntry{
	public:
		virtual EDrawable* Create()=0;
		virtual const char* GetType()=0;
		virtual const bool IsPurchasable()=0;
		virtual const int GetPrice()=0;
		virtual Sprite GetSprite()=0;
		virtual const Pos2D GetSize()=0;
	};

	template<class T>
	class EManEntry : public IManEntry{
	public:
		EManEntry(char* sType): Purchasable(T::Purchasable), Price(T::Price), pSize(T::pSize){
			eType=sType;
			DefineNewEntity(this);
		}

		const bool IsPurchasable(){
			return Purchasable;
		}
		
		const int GetPrice(){
			return Price;
		}

		Sprite GetSprite(){
			return T::GetSprite();
		}

		const Pos2D GetSize(){
			return pSize;
		}

		~EManEntry(){

		}
		EDrawable* Create(){
			T* newEnt=new T();
			newEnt->className=eType;
			return newEnt;
		}
		const char* GetType(){
			return eType;
		}
		const bool Purchasable;
		const int Price;
		const Pos2D pSize;
	private:
		char* eType;
	};

	void DefineNewEntity(IManEntry* toAdd);
	EDrawable* CreateEntity(char* eName);
	const int GetEntPrice(char* eName);
	const bool IsEntPurchasable(char* eName);
	map<string, IManEntry*>& GetEMap();
	Sprite GetEntSprite(char* eName);
	const Pos2D GetEntSize(char* eName);
}

#endif