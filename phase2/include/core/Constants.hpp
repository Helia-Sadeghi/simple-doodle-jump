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

// ══════════════════════════════════════════════════════════════════════════
// ─── PHASE 2 ADDITIONS (nothing above this line was modified) ──────────────
// ══════════════════════════════════════════════════════════════════════════

// ─── Files (phase 2) ─────────────────────────────────────────────────────────
constexpr const char* SETTINGS_FILE = "settings.txt";

// ─── Bullets ─────────────────────────────────────────────────────────────────
constexpr float BULLET_SPEED          = -800.f;
constexpr float BULLET_RADIUS_PADDING = 4.f;
constexpr float BULLET_RADIUS         = 6.f; 

constexpr float FIRE_RATE_EASY   = 0.18f;
constexpr float FIRE_RATE_MEDIUM = 0.35f;
constexpr float FIRE_RATE_HARD   = 0.35f;

// ─── Monsters ────────────────────────────────────────────────────────────────
constexpr float MONSTER_SPEED            = 70.f;

constexpr float MONSTER_SPAWN_CHANCE_EASY    = 0.10f;
constexpr float MONSTER_SPAWN_CHANCE_MEDIUM  = 0.16f;
constexpr float MONSTER_SPAWN_CHANCE_HARD    = 0.42f;

constexpr float MONSTER_PATROL_RANGE     = static_cast<float>(WINDOW_WIDTH) * 2.f;

constexpr int MONSTER_MIN_PLATFORM_GAP_EASY   = 3;
constexpr int MONSTER_MIN_PLATFORM_GAP_MEDIUM = 2;
constexpr int MONSTER_MIN_PLATFORM_GAP_HARD   = 1;

constexpr float MONSTER_MIN_SPAWN_HEIGHT = 1200.f;


constexpr float HAZARD_SIDE_GAP = PLAYER_WIDTH + 25.f;


constexpr float HAZARD_MIN_LOOKAHEAD_BUFFER = 130.f;

constexpr int MONSTER_HEALTH_EASY   = 1;
constexpr int MONSTER_HEALTH_MEDIUM = 3;
constexpr int MONSTER_HEALTH_HARD   = 6;

constexpr float MONSTER_LAND_BOOST_VELOCITY = SPRING_JUMP_VELOCITY;


constexpr float SPRING_HAZARD_SAFE_DISTANCE =
    (SPRING_JUMP_VELOCITY * SPRING_JUMP_VELOCITY) / (2.f * GRAVITY) + 80.f;


constexpr float MONSTER_HITBOX_INSET_RATIO = 0.2f;

// ─── Holes ───────────────────────────────────────────────────────────────────
constexpr float HOLE_SPAWN_CHANCE     = 0.15f;
constexpr float HOLE_MIN_SPAWN_HEIGHT = 1200.f; 
constexpr int   HOLE_MIN_PLATFORM_GAP = 2;      
constexpr float HOLE_SMALL_SCALE      = 0.7f;
constexpr float HOLE_LARGE_SCALE      = 1.15f;
constexpr float HOLE_SUCK_DURATION    = 0.9f; 

// ─── Difficulty-dependent moving-platform speed multiplier ─────────────────
constexpr float MOVING_PLATFORM_SPEED_MULT_EASY   = 1.0f;
constexpr float MOVING_PLATFORM_SPEED_MULT_MEDIUM = 1.4f;
constexpr float MOVING_PLATFORM_SPEED_MULT_HARD   = 1.4f;

// ─── Sound ───────────────────────────────────────────────────────────────────
constexpr float DEFAULT_VOLUME = 70.f; // 0..100

// ─── Combo kill bonus (fun bonus feature, not required by the spec) ────────

constexpr float COMBO_KILL_BASE_BONUS  = 25.f;
constexpr float COMBO_WINDOW_SECONDS   = 1.5f;
constexpr int   COMBO_MAX_MULTIPLIER   = 5;
constexpr float COMBO_POPUP_LIFETIME   = 0.9f;
constexpr float COMBO_POPUP_RISE_SPEED = 45.f;
constexpr unsigned int COMBO_POPUP_FONT_SIZE = 20;

// ─── UI (phase 2) ────────────────────────────────────────────────────────────
constexpr float UI_MODE_TEXT_Y_RATIO       = 0.40f; 
constexpr float UI_SETTINGS_BUTTON_Y_RATIO = 0.84f;
constexpr float UI_SETTINGS_BUTTON_SCALE   = 0.5f;  
constexpr float UI_BACK_BUTTON_Y_RATIO     = 0.92f;
constexpr float UI_BACK_BUTTON_SCALE       = 0.22f; 
constexpr float UI_SETTINGS_BEST_SCORE_Y_RATIO = 0.27f; 
constexpr float UI_VOLUME_BAR_Y_RATIO      = 0.38f;
constexpr float UI_VOLUME_BAR_WIDTH        = 260.f;
constexpr float UI_VOLUME_BAR_HEIGHT       = 14.f;
constexpr float UI_DIFFICULTY_Y_RATIO      = 0.52f;
constexpr float UI_DIFFICULTY_BUTTON_GAP   = 90.f;
constexpr float UI_DIFFICULTY_BUTTON_SIZE  = 64.f;

}

#endif
