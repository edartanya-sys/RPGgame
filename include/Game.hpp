#pragma once
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "AssetManager.hpp"
#include "TileManager.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

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
    void updatePlay(float dt);
    void updateLevelEditor(float dt);

    void HandleInput();
    void handlePlayInput();
    void handleLevelEditorInput();
    void draw();

    void initWorld();

    GameMode gameMode_ = GameMode::Play;
    ScrollWheelInput scrollWheelInput_ = ScrollWheelInput::None;

    sf::RenderWindow window_;
    Camera camera_{};

    AssetManager assetManager_{};
    TileManager tileManager_{};
    Renderer renderer_{assetManager_, tileManager_.getTileScale(), tileManager_.getBaseSize()};
    World world_{};

    sf::Vector2i mousePosWindow_{};
    sf::Vector2f mousePosView_{};

    sf::Vector2f cameraMovement_{};
};

