/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <optional>
#include "core/ResourceManager.hpp"


class SoundManager {
public:
    SoundManager();

    void loadResources();

    void setVolume(float volume0to100);
    float getVolume() const;

    void playJump();
    void playShoot();
    void playLose();

    void playMusic();
    void stopMusic();

private:
    ResourceManager<sf::SoundBuffer> buffers;

    std::optional<sf::Sound> jumpSound;
    std::optional<sf::Sound> shootSound;
    std::optional<sf::Sound> loseSound;

    sf::Music music;
    float volume;
    bool musicPlaying;
};

#endif
