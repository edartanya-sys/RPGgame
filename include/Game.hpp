#pragma once
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "AssetManager.hpp"
#include "TileManager.hpp"
#include "Renderer.hpp"

class Game {

public:
    Game();

    enum class GameMode {
        Play,
        LevelEditor
    };

    enum class ScrollWheelInput {
        Up = -1,
        None,
        Down
    };

    void run();

private:
    void updatePlay(float deltaTime);
    void updateLevelEditor(float deltaTime);

    void HandleInput();
    void handlePlayInput();
    void handleLevelEditorInput();
    void draw();

    void initWorld();

    GameMode gameMode_ = GameMode::Play;
    ScrollWheelInput scrollWheelInput_ = ScrollWheelInput::None;

    sf::RenderWindow window_;
    sf::View camera_{};

    AssetManager assetManager_{};
    Renderer renderer_{assetManager_};
    TileManager tileManager_{};
    World world_{};

    sf::Vector2i mousePosWindow_;
    sf::Vector2f mousePosView_;

    float tileScale_ = 7.f;
    int baseWidth_ = 16;
    int baseHeight_ = 16;
    int tileWidth_ = baseWidth_ * tileScale_;
    int tileHeight_ = baseHeight_ * tileScale_;
};

