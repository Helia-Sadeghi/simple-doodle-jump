/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants {

// ─── Window ─────────────────────────────────────────────────────────────────
constexpr unsigned int WINDOW_WIDTH      = 480;
constexpr unsigned int WINDOW_HEIGHT     = 720;
constexpr unsigned int FRAME_RATE_LIMIT  = 60;

// ─── Physics ─────────────────────────────────────────────────────────────────
constexpr float GRAVITY              = 900.f;
constexpr float PLAYER_MOVE_SPEED    = 340.f;
constexpr float PLAYER_JUMP_VELOCITY = -750.f;
constexpr float SPRING_JUMP_VELOCITY = -1300.f;
constexpr float MOVING_PLATFORM_SPEED = 90.f;

// ─── Entity dimensions ───────────────────────────────────────────────────────
constexpr float PLAYER_WIDTH    = 60.f;
constexpr float PLAYER_HEIGHT   = 60.f;
constexpr float PLATFORM_WIDTH  = 59.f;
constexpr float PLATFORM_HEIGHT = 18.f;

// ─── Platform generation – vertical gaps ────────────────────────────────────
constexpr float MIN_VERTICAL_GAP       = 50.f;
constexpr float MAX_VERTICAL_GAP       = 150.f;

constexpr float MAX_GAP_AFTER_BREAKABLE = 100.f;

// ─── Platform generation – type weights ─────────────────────────────────────
constexpr double NORMAL_PLATFORM_WEIGHT    = 0.62;
constexpr double MOVING_PLATFORM_WEIGHT    = 0.20;
constexpr double BREAKABLE_PLATFORM_WEIGHT = 0.18;

// ─── Platform generation – spawn helpers ────────────────────────────────────
constexpr float PLAYER_INITIAL_SPAWN_OFFSET = 60.f;

// ─── Collision detection ─────────────────────────────────────────────────────
constexpr float COLLISION_TOLERANCE      = 12.f;

constexpr float COLLISION_SINK_TOLERANCE = 1.f;

constexpr float PLATFORM_FALL_MARGIN     = 100.f;

// ─── Spring sprite-sheet ─────────────────────────────────────────────────────
constexpr int SPRING_FRAMES_COUNT        = 2;  
constexpr int SPRING_RELAXED_HEIGHT_TRIM = 9;  
constexpr float SPRING_SPAWN_CHANCE      = 0.18f;
constexpr float SPRING_COMPRESS_DURATION = 0.15f;

// ─── Camera / scrolling ──────────────────────────────────────────────────────
constexpr float SCROLL_THRESHOLD = WINDOW_HEIGHT * 0.42f;

// ─── Files ───────────────────────────────────────────────────────────────────
constexpr const char* HIGHSCORE_FILE = "highscore.txt";

// ─── UI – font sizes ─────────────────────────────────────────────────────────
constexpr unsigned int UI_TITLE_FONT_SIZE      = 40;
constexpr unsigned int UI_BEST_SCORE_FONT_SIZE = 22;
constexpr unsigned int UI_HUD_FONT_SIZE        = 24;
constexpr unsigned int UI_GAME_OVER_FONT_SIZE  = 46;

// ─── UI – outline thicknesses ────────────────────────────────────────────────
constexpr float UI_TEXT_OUTLINE_THICKNESS     = 2.f;
constexpr float UI_GAMEOVER_OUTLINE_THICKNESS = 3.f;

// ─── UI – HUD score label offset ─────────────────────────────────────────────
constexpr float UI_SCORE_TEXT_X = 10.f;
constexpr float UI_SCORE_TEXT_Y = 10.f;

// ─── UI – vertical layout (fraction of window height) ────────────────────────
constexpr float UI_TITLE_Y_RATIO          = 0.22f;
constexpr float UI_BEST_SCORE_Y_RATIO     = 0.32f;
constexpr float UI_START_BUTTON_Y_RATIO   = 0.65f;
constexpr float UI_RESTART_BUTTON_Y_RATIO = 0.62f;
constexpr float UI_MENU_BUTTON_Y_RATIO    = 0.74f;
constexpr float UI_GAME_OVER_TEXT_Y_RATIO = 0.30f;
constexpr float UI_FINAL_SCORE_Y_RATIO    = 0.42f;
constexpr float UI_FINAL_BEST_Y_RATIO     = 0.50f;

} 

#endif
