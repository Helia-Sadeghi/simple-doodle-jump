/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/SoundManager.hpp"
#include "core/Constants.hpp"
#include <stdexcept>

SoundManager::SoundManager() : volume(Constants::DEFAULT_VOLUME), musicPlaying(false) {
}

void SoundManager::loadResources() {
    sf::SoundBuffer& jumpBuffer  = buffers.load("jump",  "sounds/Jumping_Sound.wav");
    sf::SoundBuffer& shootBuffer = buffers.load("shoot", "sounds/Shooting_Sound.wav");
    sf::SoundBuffer& loseBuffer  = buffers.load("lose",  "sounds/Loosing_Sound.wav");

    jumpSound.emplace(jumpBuffer);
    shootSound.emplace(shootBuffer);
    loseSound.emplace(loseBuffer);

    if (!music.openFromFile("sounds/MainMenu_Song.flac")) {
        throw std::runtime_error("SoundManager: failed to load background music");
    }
    music.setLooping(true);

    setVolume(volume);
}

void SoundManager::setVolume(float newVolume) {
    volume = newVolume;
    if (jumpSound)  jumpSound->setVolume(volume);
    if (shootSound) shootSound->setVolume(volume);
    if (loseSound)  loseSound->setVolume(volume);
    music.setVolume(volume);
}

float SoundManager::getVolume() const {
    return volume;
}

void SoundManager::playJump() {
    if (jumpSound) jumpSound->play();
}

void SoundManager::playShoot() {
    if (shootSound) shootSound->play();
}

void SoundManager::playLose() {
    if (loseSound) loseSound->play();
}

void SoundManager::playMusic() {
    if (!musicPlaying) {
        music.play();
        musicPlaying = true;
    }
}

void SoundManager::stopMusic() {
    music.stop();
    musicPlaying = false;
}
