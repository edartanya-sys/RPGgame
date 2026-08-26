#include <fstream>
#include <random>
#include <iostream>
#include "Game.hpp"

#include "CollisionSystem.hpp"
#include "Components.hpp"
#include "EnemyAISystem.hpp"
#include "MovementSystem.hpp"
#include "MathFunctions.hpp"

Game::Game() : window_(sf::VideoMode({1600, 1080}), "Soslo") {

    window_.setFramerateLimit(120);
    camera_.setSize({1600, 1080});
    tileManager_.loadMapFromFile("level1.map");
}

void Game::run() {
    sf::Clock clock;
    initWorld();

    while (window_.isOpen()) {
        HandleInput();

        float dt = clock.restart().asSeconds();
        switch (gameMode_) {
            case GameMode::Play:
                updatePlay(dt);
                break;
            case GameMode::LevelEditor:
                updateLevelEditor(dt);
                break;
        }

        draw();
    }
}

void Game::draw() {
    window_.clear({80, 80, 80});
    window_.setView(camera_);
    mousePosWindow_ = sf::Mouse::getPosition(window_);
    mousePosView_ = window_.mapPixelToCoords(mousePosWindow_);

    renderer_.drawBounds(window_, tileManager_);
    renderer_.drawTiles(window_, tileManager_);

    switch (gameMode_) {
        case GameMode::Play:
            break;
        case GameMode::LevelEditor:
            renderer_.drawLevelEditor(window_, tileManager_, mousePosView_);
            break;
    }

    renderer_.drawEntities(window_, world_);

    window_.setView(window_.getDefaultView());

    window_.display();
}

namespace {
    Entity getPlayer(World &world) {
        auto& players = world.storage<Player>();
        assert(players.getSize() == 1);
        return players.getEntityAt(0);
    }
}

void Game::initWorld() {
    const sf::Texture &playerTexture = assetManager_.getTexture(TextureID::Player);
    sf::Sprite playerSprite{playerTexture};
    sf::Vector2f scale = {5.f, 5.f};
    playerSprite.setScale(scale);
    auto bounds = playerSprite.getLocalBounds();
    playerSprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    Entity player = world_.create();
    world_.addComponent(player, SpriteComponent{playerSprite});
    world_.addComponent(player, Position{{200.f, 100.f}});
    world_.addComponent(player, Player{});
    world_.addComponent(player, Velocity{{0.f, 0.f}, 500.f});
    world_.addComponent(player, HealthComponent{100});
    sf::Vector2f size{bounds.size.x * scale.x * 0.3f, bounds.size.y * scale.y * 0.6f};
    sf::Vector2f offset{-size.x / 2.f, -size.y / 2.f};
    world_.addComponent(player, Collider{sf::FloatRect{offset, size}});
    camera_.setCenter(world_.storage<Position>().get(player).value);

    const sf::Texture &ratTexture = assetManager_.getTexture(TextureID::Rat);
    sf::Sprite ratSprite{ratTexture};
    ratSprite.setScale({7.f, 7.f});
    bounds = ratSprite.getLocalBounds();
    ratSprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    Entity rat = world_.create();
    world_.addComponent(rat, SpriteComponent{ratSprite});
    world_.addComponent(rat, Position{{500.f, 400.f}});
    world_.addComponent(rat, Velocity{{0.f, 0.f}, 400.f});
    world_.addComponent(rat, Enemy{});
    world_.addComponent(rat, ChaseComponent{});
}

void Game::updatePlay(float dt) {
    systems::updateEnemyAI(world_);
    auto collisions = systems::updateCollisions(world_);
    systems::updateMovement(world_, dt);
    Entity player = getPlayer(world_);
    camera_.setCenter(world_.storage<Position>().get(player).value);
}

void Game::updateLevelEditor(float dt) {
    // camera_.move({90.f * dt, 90.f * dt});
}

void Game::HandleInput() {
    scrollWheelInput_ = ScrollWheelInput::None;
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
        if (const auto *scroll = event->getIf<
            sf::Event::MouseWheelScrolled>()) {
            if (scroll->wheel == sf::Mouse::Wheel::Vertical) {
                if (scroll->delta > 0) {
                    scrollWheelInput_ = ScrollWheelInput::Up;
                }
                else if (scroll->delta < 0) {
                    scrollWheelInput_ = ScrollWheelInput::Down;
                }
            }
        }
        if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            switch (key->scancode) {
                case sf::Keyboard::Scancode::T: {
                    if (gameMode_ == GameMode::Play) {
                        gameMode_ = GameMode::LevelEditor;
                    }
                    else if (gameMode_ == GameMode::LevelEditor) {
                        gameMode_ = GameMode::Play;
                    }
                    break;
                }
                case sf::Keyboard::Scancode::O: {
                    if (gameMode_ == GameMode::LevelEditor) {
                        tileManager_.saveMapToFile("level1.map");
                    }
                    break;
                }
                case sf::Keyboard::Scancode::Num1:
                case sf::Keyboard::Scancode::Num2:
                case sf::Keyboard::Scancode::Num3:
                case sf::Keyboard::Scancode::Num4:
                case sf::Keyboard::Scancode::Num5:
                case sf::Keyboard::Scancode::Num6:
                case sf::Keyboard::Scancode::Num7:
                case sf::Keyboard::Scancode::Num8:
                case sf::Keyboard::Scancode::Num9:
                case sf::Keyboard::Scancode::Num0: {
                    tileManager_.tileGroupOption_ = static_cast<TileID>((math::toIndex(key->scancode) - 26));
                    tileManager_.tileOption_ = 0;
                }
                break;

                default:
                    break;
            }
        }
    }

    switch (gameMode_) {
        case GameMode::Play:
            handlePlayInput();
            break;
        case GameMode::LevelEditor:
            handleLevelEditorInput();
            break;
    }
}

void Game::handlePlayInput() {

    sf::Vector2f velocity = {0.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        velocity.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        velocity.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        velocity.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        velocity.x += 1.f;
    }

    Entity player = getPlayer(world_);
    world_.storage<Velocity>().get(player).vector = velocity;
}

void Game::handleLevelEditorInput() {
    tileManager_.changeTileOption(math::toIndex(scrollWheelInput_));

    int x = std::floor(mousePosView_.x / tileWidth_);
    int y = std::floor(mousePosView_.y / tileHeight_);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        tileManager_.createTile(x, y);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        tileManager_.deleteTile(x, y);
    }
}
