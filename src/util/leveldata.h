#ifndef _LEVELDATA_HEADER_
#define _LEVELDATA_HEADER_

#include "util/files.h"
#include "entities/entity.h"
#include <map>
#include <list>

namespace Util{
	struct EntData{
		std::string eName;
		std::string excess;
		Pos2D position;
	};

	struct LevelData{
		Pos2D size;
		std::string fileName;
		std::vector<EntData> entTable;
		std::map<std::string, int> eLimits;
	};

	LevelData* GetLevelData(const char* lName);
	void LoadLevel(char* fileName);
	void LoadAllLevels();

	//Campaigns

	struct CampaignData{
		std::string campaignName;
		std::list<LevelData*> levels;
	};

	void LoadCampaign(const char* lName);
	void LoadAllCampaigns();
	CampaignData* GetCampaignData(const char* cName);
}



#endif