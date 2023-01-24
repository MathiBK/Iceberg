//
// Created by Axel on 20/11/2019.
//

#include <jsoncpp/json/json.h>

#include <utility>
#include <iostream>
#include <fstream>
#include "save_manager.h"

SaveManager::SaveManager()
{

    ///Save data
    m_xp = 0;
    m_lvl = 0;
    m_usedLvls = 1;
    m_enemiesDefeatedId = {};
    m_profileName = "uninitialized";
    m_abilitiesConfigured = {{"q", "none"}, {"w", "none"}, {"e", "none"}};
    m_abilitiesUnlocked = {};
    m_checkPos = {0,0};

    ///Config data
    m_screenWidth = 1600;
    m_screenHeight = 900;
    m_framerate = 60;
    m_fullscreen = -1;
}

SaveManager::SaveManager(std::string name)
{
    this->m_profileName = std::move(name);
    m_xp = 0;
    m_lvl = 0;
    m_usedLvls = 1;
    m_enemiesDefeatedId = {};
    m_abilitiesConfigured = {{"q", " "}, {"w", " "}, {"e", " "}};
    m_abilitiesUnlocked = {};
    m_checkPos = {0,0};
    m_framerate = 60;
    m_screenWidth = 1600;
    m_screenHeight = 900;
    m_fullscreen = -1;
}


bool SaveManager::save()
{
    Json::Value saveRoot;
    Json::StreamWriterBuilder swb;

    std::ofstream file;
    file.open("saves/save.json", std::ios::out);

    //dette gjør det vanskeligere å lese savefilen for mennesker
    //swb.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(swb.newStreamWriter());


    Json::Value enemiesDefeatedVec(Json::arrayValue);
    Json::Value spawnPos(Json::arrayValue);
    Json::Value abilitiesUnlocked(Json::arrayValue);


    for(auto& i : m_enemiesDefeatedId)
        enemiesDefeatedVec.append(i);

    spawnPos.append(m_checkPos.x);
    spawnPos.append(m_checkPos.y);

    for(auto &i : m_abilitiesUnlocked)
        abilitiesUnlocked.append(i);


    saveRoot["gameSaveData"]["xp"] = m_xp;
    saveRoot["gameSaveData"]["lvl"] = m_lvl;
    saveRoot["gameSaveData"]["usedLvls"] = m_usedLvls;
    saveRoot["gameSaveData"]["enemiesDefeated"] = enemiesDefeatedVec;
    saveRoot["gameSaveData"]["spawnPos"] = spawnPos;
    saveRoot["gameSaveData"]["abilitiesConfigured"]["q"] = m_abilitiesConfigured.at("q");
    saveRoot["gameSaveData"]["abilitiesConfigured"]["w"] = m_abilitiesConfigured.at("w");
    saveRoot["gameSaveData"]["abilitiesConfigured"]["e"] = m_abilitiesConfigured.at("e");
    saveRoot["gameSaveData"]["abilitiesUnlocked"] = abilitiesUnlocked;

    saveRoot["gameSaveData"]["profileName"] = m_profileName;

    saveRoot["gameSaveData"]["framerate"] = m_framerate;
    saveRoot["gameSaveData"]["fullscreen"] = m_fullscreen;
    saveRoot["configData"]["screenWidth"]= m_screenWidth;
    saveRoot["configData"]["screenHeight"]= m_screenHeight;

    writer->write(saveRoot, &file);
    file.close();

}

void SaveManager::setUsedLvls(int lvls)
{
    this->m_usedLvls = lvls;
}

void SaveManager::setXp(int xp, int lvl)
{
    this->m_xp = xp;
    this->m_lvl = lvl;
}

void SaveManager::setEnemiesDefeated(const std::list<int>& en)
{
    this->m_enemiesDefeatedId = en;
}
void SaveManager::setEnemiesDiedToo(const std::list<int>& en)
{
    this->m_enemiesDiedTooId = en;
}



bool SaveManager::load(std::string& filename)
{

    std::ifstream file(filename);


    Json::Value saveRoot;
    Json::CharReaderBuilder builder;

    std::string errs;
    bool parseSuccessful = Json::parseFromStream(builder, file, &saveRoot, &errs);

    if(!parseSuccessful)
        return false;

    ///Save data
    this->m_xp = saveRoot["gameSaveData"]["xp"].asInt();
    this->m_lvl = saveRoot["gameSaveData"]["lvl"].asInt();
    this->m_usedLvls = saveRoot["gameSaveData"]["usedLvls"].asInt();

    for(size_t i = 0; i < saveRoot["gameSaveData"]["enemiesDefeated"].size(); i++)
        this->m_enemiesDefeatedId.emplace_back(saveRoot["gameSaveData"]["enemiesDefeated"][(int) i].asUInt());

    this->m_checkPos.x = saveRoot["gameSaveData"]["spawnPos"][(int)0].asInt();
    this->m_checkPos.y = saveRoot["gameSaveData"]["spawnPos"][(int)1].asInt();
    this->m_abilitiesConfigured.at("q") =  saveRoot["gameSaveData"]["abilitiesConfigured"]["q"].asString();
    this->m_abilitiesConfigured.at("w") =  saveRoot["gameSaveData"]["abilitiesConfigured"]["w"].asString();
    this->m_abilitiesConfigured.at("e") =  saveRoot["gameSaveData"]["abilitiesConfigured"]["e"].asString();

    for(size_t i = 0; i < saveRoot["gameSaveData"]["abilitiesUnlocked"].size(); i++)
        this->m_abilitiesUnlocked.emplace_back(saveRoot["gameSaveData"]["abilitiesUnlocked"][(int) i].asString());

    this->m_profileName = saveRoot["gameSaveData"]["profileName"].asString();

    ///Config data
    this->m_framerate = saveRoot["gameSaveData"]["framerate"].asInt();
    this->m_fullscreen = saveRoot["gameSaveData"]["fullscreen"].asInt();
    this->m_screenWidth = saveRoot["configData"]["screenWidth"].asUInt();
    this->m_screenHeight = saveRoot["configData"]["screenHeight"].asUInt();

    file.close();

    return true;
}

bool SaveManager::loadSettings(std::string& filename)
{

    std::ifstream file(filename);


    Json::Value saveRoot;
    Json::CharReaderBuilder builder;

    std::string errs;
    bool parseSuccessful = Json::parseFromStream(builder, file, &saveRoot, &errs);

    if(!parseSuccessful)
        return false;

    ///Config data
    this->m_framerate = saveRoot["gameSaveData"]["framerate"].asInt();
    this->m_fullscreen = saveRoot["gameSaveData"]["fullscreen"].asInt();
    this->m_screenWidth = saveRoot["configData"]["screenWidth"].asUInt();
    this->m_screenHeight = saveRoot["configData"]["screenHeight"].asUInt();

    file.close();

    return true;
}

