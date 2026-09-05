/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <string>
#include <array>
#include "core/Difficulty.hpp"

class ScoreManager {
public:
    explicit ScoreManager(std::string highscoreFilePath);


    void setDifficulty(Difficulty difficulty);
    Difficulty getDifficulty() const;

    void reset();
    void updateFromHeight(float height);
    void finalizeRun();

    void addBonus(float amount);

    float getScore() const;                            
    float getBestScore() const;                       
    float getBestScore(Difficulty difficulty) const;

private:
    void loadBestScores();
    void saveBestScores() const;

    std::string highscoreFilePath;
    float score;      
    float bonusScore;
    std::array<float, 3> bestScores; 
    Difficulty currentDifficulty;
};

#endif
