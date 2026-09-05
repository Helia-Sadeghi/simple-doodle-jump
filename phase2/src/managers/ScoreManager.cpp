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
    : highscoreFilePath(std::move(filePath)), score(0.f), bonusScore(0.f),
      bestScores{0.f, 0.f, 0.f}, currentDifficulty(Difficulty::Easy) {
    loadBestScores();
}

void ScoreManager::setDifficulty(Difficulty difficulty) {
    currentDifficulty = difficulty;
}

Difficulty ScoreManager::getDifficulty() const {
    return currentDifficulty;
}

void ScoreManager::reset() {
    score = 0.f;
    bonusScore = 0.f;
}

void ScoreManager::updateFromHeight(float height) {
    score = height;
}

void ScoreManager::addBonus(float amount) {
    bonusScore += amount;
}

void ScoreManager::finalizeRun() {
    std::size_t index = static_cast<std::size_t>(currentDifficulty);
    float total = getScore();
    if (total > bestScores[index]) {
        bestScores[index] = total;
        saveBestScores();
    }
}

float ScoreManager::getScore() const {
    return score + bonusScore;
}

float ScoreManager::getBestScore() const {
    return bestScores[static_cast<std::size_t>(currentDifficulty)];
}

float ScoreManager::getBestScore(Difficulty difficulty) const {
    return bestScores[static_cast<std::size_t>(difficulty)];
}

void ScoreManager::loadBestScores() {
    std::ifstream file(highscoreFilePath);
    if (!file.is_open()) {
        return;
    }


    float first = 0.f;
    if (!(file >> first)) {
        return;
    }

    float second = 0.f;
    float third  = 0.f;
    if (file >> second >> third) {
        bestScores[0] = first;
        bestScores[1] = second;
        bestScores[2] = third;
    } else {
        bestScores[0] = first;
        bestScores[1] = first;
        bestScores[2] = first;
    }
}

void ScoreManager::saveBestScores() const {
    std::ofstream file(highscoreFilePath, std::ios::trunc);
    if (file.is_open()) {
        file << static_cast<int>(bestScores[0]) << "\n"
             << static_cast<int>(bestScores[1]) << "\n"
             << static_cast<int>(bestScores[2]) << "\n";
    }
}
