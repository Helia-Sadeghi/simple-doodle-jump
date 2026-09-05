# Simple Doodle Jump — Phase 1

A C++ implementation of the classic *Doodle Jump* game, built with the SFML library. This initial phase covers core gameplay: platform generation, player movement and jumping, and score tracking.

## Build and Run

### Windows (MSYS2 / MinGW-w64)

Install the required dependencies:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-sfml
```

Build the project:

```bash
mingw32-make
```

Make sure the required SFML DLLs are placed next to `SimpleDoodleJump.exe`, or add the SFML `bin` directory to your system `PATH`.

### Linux / macOS

Build the project:

```bash
make
```

Run the game:

```bash
./SimpleDoodleJump
```

> **Note:** Run the program from the `simple-doodle-jump` directory, as the game relies on relative paths for `assets/`, `fonts/`, and `highscore.txt`.

## Project Structure

```text
simple-doodle-jump/
├── assets/
├── fonts/
├── include/
│   ├── core/
│   │   ├── Constants.hpp
│   │   ├── ResourceManager.hpp
│   │   └── Game.hpp
│   ├── entities/
│   │   ├── Player.hpp
│   │   ├── Platform.hpp
│   │   ├── NormalPlatform.hpp
│   │   ├── MovingPlatform.hpp
│   │   ├── BreakablePlatform.hpp
│   │   └── Spring.hpp
│   └── managers/
│       ├── PlatformManager.hpp
│       ├── UiManager.hpp
│       └── ScoreManager.hpp
├── src/
│   ├── core/
│   │   └── Game.cpp
│   ├── entities/
│   │   ├── Player.cpp
│   │   ├── Platform.cpp
│   │   ├── NormalPlatform.cpp
│   │   ├── MovingPlatform.cpp
│   │   ├── BreakablePlatform.cpp
│   │   └── Spring.cpp
│   ├── managers/
│   │   ├── PlatformManager.cpp
│   │   ├── UiManager.cpp
│   │   └── ScoreManager.cpp
│   └── main.cpp
├── highscore.txt
└── Makefile
```

## Architecture

| Component | Responsibility |
|---|---|
| **Game** (core) | Manages the main game loop, game states (`Menu`, `Gameplay`, `GameOver`), the game window, and the view. |
| **PlatformManager** | Handles platform generation, updates, rendering, collision detection, and removal. Platforms are stored in a `std::vector<std::unique_ptr<Platform>>`. |
| **UiManager** | Manages sprites and text elements for the menu, HUD, and Game Over screen, as well as button interactions. |
| **ScoreManager** | Tracks the current score and high score, including saving/loading `highscore.txt`. |
| **Player / Platform / Spring** | Represent the core game entities and their individual behaviors. |

## Polymorphism and Memory Management

- `Platform` is an abstract base class. `NormalPlatform`, `MovingPlatform`, and `BreakablePlatform` inherit from it and override their specific behavior.
- `PlatformManager` creates and stores platforms as `std::unique_ptr<Platform>` inside a `std::vector`. Platforms are removed using the erase-remove idiom, while `unique_ptr` automatically manages memory — eliminating the need for manual `new`/`delete`.
- `Spring` objects are managed with `std::unique_ptr<Spring>` and are owned by their corresponding `Platform`.
- `dynamic_cast` is used to identify `NormalPlatform`, the only platform type that can contain a spring.