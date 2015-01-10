#include "util/leveldata.h"
#include <iostream>
#include <fstream>


using namespace std;

namespace Util{
	map<string, LevelData> gLevels;

	LevelData* GetLevelData(const char* lName){
		map<string, LevelData>::iterator it;
		it=gLevels.find(lName);
		if(it==gLevels.end()){
			return NULL;
		}
		return &it->second;
	}

	void LoadLevel(char* fileName){
		ifstream levelFile(fileName);
		if(!levelFile.is_open()){
			return;
		}
		int line=0;
		string sLine;
		LevelData lData;
		while(levelFile.good()){
			getline(levelFile, sLine);

			if(line==0){
				if(sLine.compare("leveldatah")!=0){
					return;
				}
			} else if(line==1){
				int sizeX=atoi(sLine.c_str());
				lData.size.x=sizeX;
				line++;
				continue;
			} else if(line==2){
				int sizeY=atoi(sLine.c_str());
				lData.size.y=sizeY;
				line++;
				continue;
			} else if(sLine.compare(0, 2, "l|")==0){
				char* lStr=new char[sLine.size()+2];
				strcpy(lStr, sLine.substr(2).c_str());
				char *tok;
				tok=strtok(lStr, ":");
				string eName=tok;
				tok=strtok(NULL, ":");
				int limit=atoi(tok);
				lData.eLimits[eName]=limit;
				delete[] lStr;
			} else {
				EntData eData;
				char *entStr=new char[sLine.size()+2];
				strcpy(entStr, sLine.c_str());
				char *tok;
				tok=strtok(entStr, ":");
				eData.eName=tok;
				tok=strtok(NULL, ":");
				eData.position.x=atoi(tok);
				tok=strtok(NULL, ":");
				eData.position.y=atoi(tok);
				tok=strtok(NULL, ":");
				if(tok!=NULL){
					eData.excess=tok;
				}
				delete[] entStr;
				lData.entTable.push_back(eData);
			}

			line++;
		}
		levelFile.close();
		char fStr[100];
		strcpy(fStr, fileName);
		char* tok;
		tok=strtok(fStr, "\\.");
		tok=strtok(NULL, "\\.");
		string levelName=tok;
		lData.fileName=levelName;
		printf("Loading: %s\n", levelName.c_str());
		gLevels[levelName]=lData;
	}

	void LoadAllLevels(){
		vector<string> levelFiles=GetDirectoryContents("levels\\");
		vector<string>::iterator it;
		for(it=levelFiles.begin(); it<levelFiles.end(); it++){
			char *lName=new char[it->size()+10];
			strcpy(lName, "levels\\");
			strcat(lName, it->c_str());
			LoadLevel(lName);
			delete[] lName;
		}
	}

	map<string, CampaignData> gCampaigns;

	void LoadCampaign(const char* lName){
		ifstream levelFile(lName);
		if(!levelFile.is_open()){
			return;
		}
		int line=0;
		CampaignData newCampaign;
		string sLine;
		while(levelFile.good()){
			getline(levelFile, sLine);
			LevelData* lData=GetLevelData(sLine.c_str());
			if(lData){
				newCampaign.levels.push_back(lData);
			}
		}
		char fStr[100];
		strcpy(fStr, lName);
		char* tok;
		tok=strtok(fStr, "\\.");
		tok=strtok(NULL, "\\.");
		string campaignName=tok;
		newCampaign.campaignName=campaignName;
		gCampaigns[campaignName]=newCampaign;
		printf("wat%d\n", gCampaigns.size());
	}

	void LoadAllCampaigns(){
		vector<string> levelFiles=GetDirectoryContents("campaigns\\");
		vector<string>::iterator it;
		for(it=levelFiles.begin(); it<levelFiles.end(); it++){
			char *lName=new char[it->size()+12];
			strcpy(lName, "campaigns\\");
			strcat(lName, it->c_str());
			LoadCampaign(lName);
			delete[] lName;
		}
	}

	CampaignData* GetCampaignData(const char* cName){
		map<string, CampaignData>::iterator it;
		it=gCampaigns.find(cName);
		if(it==gCampaigns.end()){
			return NULL;
		}
		return &it->second;
	}
}