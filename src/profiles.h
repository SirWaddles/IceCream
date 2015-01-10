#ifndef _PROFILE_HEADER_
#define _PROFILE_HEADER_

#include <string>
#include <map>

namespace Profiles{

	enum Faction{
		FACTION_UNASSIGNED=0,
		FACTION_ICE_CREAM,
		FACTION_FIRE_BALL
	};

	class GProfile {
	public:
		GProfile(char* name);
		Faction GetFaction();
		void SetFaction(Faction fact);
		void SetCampaign(char* campaign);
		const char* GetCampaign();
		
		const char* GetNextLevel();
		void FinishedLevel(const char* level);

		void SetMoney(int money);
		int GetMoney();
		void AddMoney(int add);
		bool CanAfford(int money);
	private:
		int Money;
		Faction pFaction;
		char* pName;
		std::string activeCampaign;
		//Campaign ID, Level ID.
		std::map<std::string, std::string> campaignStates;
	};

	void AddNewProfile(char* name);
	void EraseProfle(char* name);
	bool SelectProfile(char* name);
	GProfile* GetActiveProfile();
}


#endif