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

class ScoreManager {
public:
    explicit ScoreManager(std::string highscoreFilePath);

    void reset();
    void updateFromHeight(float height);
    void finalizeRun();

    float getScore() const;
    float getBestScore() const;

private:
    float loadBestScore() const;
    void saveBestScore(float value) const;

    std::string highscoreFilePath;
    float score;
    float bestScore;
};

#endif
