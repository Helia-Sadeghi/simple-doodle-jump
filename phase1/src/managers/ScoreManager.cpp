/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/ScoreManager.hpp"
#include <fstream>

ScoreManager::ScoreManager(std::string filePath)
    : highscoreFilePath(std::move(filePath)), score(0.f), bestScore(0.f) {
    bestScore = loadBestScore();
}

void ScoreManager::reset() {
    score = 0.f;
}

void ScoreManager::updateFromHeight(float height) {
    score = height;
}

void ScoreManager::finalizeRun() {
    if (score > bestScore) {
        bestScore = score;
        saveBestScore(bestScore);
    }
}

float ScoreManager::getScore() const {
    return score;
}

float ScoreManager::getBestScore() const {
    return bestScore;
}

float ScoreManager::loadBestScore() const {
    std::ifstream file(highscoreFilePath);
    float value = 0.f;
    if (file.is_open()) {
        file >> value;
    }
    return value;
}

void ScoreManager::saveBestScore(float value) const {
    std::ofstream file(highscoreFilePath, std::ios::trunc);
    if (file.is_open()) {
        file << static_cast<int>(value);
    }
}
