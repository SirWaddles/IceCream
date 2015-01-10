#include "profiles.h"
#include "util/leveldata.h"
#include <map>
#include <string>

using namespace std;

namespace Profiles{

	map<string, GProfile*> prfCont;
	GProfile* activeProfile=NULL;

	Faction GProfile::GetFaction(){
		return pFaction;
	}

	void GProfile::SetFaction(Faction fact){
		pFaction=fact;
	}

	GProfile::GProfile(char* name){
		pFaction=FACTION_ICE_CREAM;
		pName=name;
		Money=0;
	}

	void GProfile::SetCampaign(char* campaign){
		activeCampaign=campaign;
	}

	const char* GProfile::GetCampaign(){
		return activeCampaign.c_str();
	}

	const char* GProfile::GetNextLevel(){
		Util::CampaignData* cData=Util::GetCampaignData(activeCampaign.c_str());
		map<string, string>::iterator it;
		it=campaignStates.find(cData->campaignName);
		if(it==campaignStates.end()){
			return (*cData->levels.begin())->fileName.c_str();
		}
		string activeLevel=it->second;
		list<Util::LevelData*>::iterator it2;
		for(it2=cData->levels.begin(); it2!=cData->levels.end(); it2++){
			if((*it2)->fileName.compare(activeLevel)==0){
				it2++;
				if(it2==cData->levels.end()){
					return NULL;
				} else {
					return (*it2)->fileName.c_str();
				}
			}
		}
		return NULL;
	}

	void GProfile::FinishedLevel(const char* level){
		char* buf=new char[100];
		strcpy(buf, level);
		campaignStates[activeCampaign]=string(buf);
		delete buf;
	}

	void AddNewProfile(char* name){
		GProfile* newPrf = new GProfile(name);
		prfCont[name]=newPrf;
	}

	void EraseProfle(char* name){
		map<string, GProfile*>::iterator it=prfCont.find(name);
		if(it!=prfCont.end()){
			delete it->second;
			prfCont.erase(it);
		}
	}

	bool SelectProfile(char* name){
		map<string, GProfile*>::iterator it=prfCont.find(name);
		if(it!=prfCont.end()){
			activeProfile=it->second;
			return true;
		} 
		return false;
	}

	GProfile* GetActiveProfile(){
		return activeProfile;
	}

	/*void SetMoney(int money);
		int GetMoney();
		void AddMoney(int add);
		bool CanAfford(int money);*/

	void GProfile::SetMoney(int money){
		if(money<0){
			Money=0;
			return;
		}
		Money=money;
	}

	int GProfile::GetMoney(){
		return Money;
	}

	void GProfile::AddMoney(int add){
		if(CanAfford(-add)){
			SetMoney(GetMoney()+add);
		}
	}

	bool GProfile::CanAfford(int money){
		return (money<=Money);
	}

}