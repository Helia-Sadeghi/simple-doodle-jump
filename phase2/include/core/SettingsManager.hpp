/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef SETTINGS_MANAGER_HPP
#define SETTINGS_MANAGER_HPP

#include <string>
#include "core/Difficulty.hpp"

class SettingsManager {
public:
    explicit SettingsManager(std::string settingsFilePath);

    float getVolume() const;
    void setVolume(float volume0to100);

    Difficulty getDifficulty() const;
    void setDifficulty(Difficulty newDifficulty);

    void save() const;

private:
    void load();

    std::string settingsFilePath;
    float volume;
    Difficulty difficulty;
};

#endif
