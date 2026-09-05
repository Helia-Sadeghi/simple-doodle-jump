/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "core/SettingsManager.hpp"
#include "core/Constants.hpp"
#include <fstream>
#include <algorithm>

SettingsManager::SettingsManager(std::string filePath)
    : settingsFilePath(std::move(filePath)), volume(Constants::DEFAULT_VOLUME), difficulty(Difficulty::Easy) {
    load();
}

float SettingsManager::getVolume() const {
    return volume;
}

void SettingsManager::setVolume(float newVolume) {
    volume = std::clamp(newVolume, 0.f, 100.f);
}

Difficulty SettingsManager::getDifficulty() const {
    return difficulty;
}

void SettingsManager::setDifficulty(Difficulty newDifficulty) {
    difficulty = newDifficulty;
}

void SettingsManager::load() {
    std::ifstream file(settingsFilePath);
    if (!file.is_open()) {
        return; // keep defaults; file will be created on first save()
    }

    float storedVolume = volume;
    int storedDifficulty = 0;
    if (file >> storedVolume >> storedDifficulty) {
        volume = std::clamp(storedVolume, 0.f, 100.f);
        if (storedDifficulty >= 0 && storedDifficulty <= 2) {
            difficulty = static_cast<Difficulty>(storedDifficulty);
        }
    }
}

void SettingsManager::save() const {
    std::ofstream file(settingsFilePath, std::ios::trunc);
    if (file.is_open()) {
        file << static_cast<int>(volume) << "\n" << static_cast<int>(difficulty) << "\n";
    }
}
