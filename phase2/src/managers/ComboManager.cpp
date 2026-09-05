/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/ComboManager.hpp"
#include "core/Constants.hpp"
#include <algorithm>
#include <sstream>
#include <cmath>

ComboManager::Popup::Popup(const sf::Font& font, const std::string& content, unsigned int fontSize,
                            const sf::Vector2f& position)
    : text(font, content, fontSize), age(0.f) {
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition(position);
}

ComboManager::ComboManager(ResourceManager<sf::Font>& fontManager)
    : fonts(fontManager), comboCount(0), timeSinceLastKill(Constants::COMBO_WINDOW_SECONDS + 1.f) {
}

float ComboManager::registerKill(const sf::Vector2f& worldPosition) {
    // Still within the combo window since the last kill -> extend the
    // streak; otherwise this kill starts a brand new combo.
    comboCount = (timeSinceLastKill <= Constants::COMBO_WINDOW_SECONDS) ? comboCount + 1 : 1;
    timeSinceLastKill = 0.f;

    int multiplier = std::min(comboCount, Constants::COMBO_MAX_MULTIPLIER);
    float bonus = Constants::COMBO_KILL_BASE_BONUS * static_cast<float>(multiplier);

    std::ostringstream oss;
    oss << "+" << static_cast<int>(bonus);
    if (multiplier > 1) {
        oss << " x" << multiplier;
    }

    Popup popup(fonts.get("main"), oss.str(), Constants::COMBO_POPUP_FONT_SIZE, worldPosition);
    // Streaks pop yellow so a growing combo is satisfying to watch build up.
    popup.text.setFillColor(multiplier >= 2 ? sf::Color::Yellow : sf::Color::White);
    popups.push_back(std::move(popup));

    return bonus;
}

void ComboManager::update(float deltaTime) {
    timeSinceLastKill += deltaTime;
    if (timeSinceLastKill > Constants::COMBO_WINDOW_SECONDS) {
        comboCount = 0; // streak expired; the next kill starts fresh at x1
    }

    for (Popup& popup : popups) {
        popup.age += deltaTime;

        sf::Vector2f position = popup.text.getPosition();
        position.y -= Constants::COMBO_POPUP_RISE_SPEED * deltaTime;
        popup.text.setPosition(position);

        float t = std::min(1.f, popup.age / Constants::COMBO_POPUP_LIFETIME);
        sf::Color fillColor = popup.text.getFillColor();
        fillColor.a = static_cast<std::uint8_t>(255.f * (1.f - t));
        popup.text.setFillColor(fillColor);
        sf::Color outlineColor = popup.text.getOutlineColor();
        outlineColor.a = fillColor.a;
        popup.text.setOutlineColor(outlineColor);
    }

    popups.erase(std::remove_if(popups.begin(), popups.end(),
                                 [](const Popup& popup) { return popup.age >= Constants::COMBO_POPUP_LIFETIME; }),
                 popups.end());
}

void ComboManager::render(sf::RenderWindow& window) const {
    for (const Popup& popup : popups) {
        window.draw(popup.text);
    }
}

void ComboManager::reset() {
    popups.clear();
    comboCount = 0;
    timeSinceLastKill = Constants::COMBO_WINDOW_SECONDS + 1.f;
}

int ComboManager::getComboCount() const {
    return comboCount;
}
