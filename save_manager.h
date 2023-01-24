//
// Created by Axel on 20/11/2019.
//

#ifndef ICEBERG_SAVE_MANAGER_H
#define ICEBERG_SAVE_MANAGER_H

#include <list>
#include <SFML/System.hpp>

class SaveManager
{
public:
    SaveManager();
    explicit SaveManager(std::string name);



    ///Config data

    unsigned int m_screenWidth;
    unsigned int m_screenHeight;

    ///Save data
    std::list<int> m_enemiesDefeatedId;
    std::list<int> m_enemiesDiedTooId;
    std::string m_profileName;
    sf::Vector2f m_checkPos;
    std::list<std::string> m_abilitiesUnlocked;
    std::map<std::string, std::string> m_abilitiesConfigured;

    int m_xp{};
    int m_lvl{};
    int m_usedLvls{};
    int m_framerate{};
    int m_fullscreen{};

    void setUsedLvls(int lvls);
    void setXp(int xp, int lvl);
    void setEnemiesDefeated(const std::list<int>& en);
    void setEnemiesDiedToo(const std::list<int>& en);
    bool save();
    bool load(std::string& filename);
    bool loadSettings(std::string& filename);


};


#endif //ICEBERG_SAVE_MANAGER_H
