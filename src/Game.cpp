#include "Game.hpp"
#include "MathFunctions.hpp"
#include <iostream>
#include <fstream>

#include "Entity.hpp"

Game::Game()
    : window_(sf::VideoMode({1600, 1080}), "Soslo")
    , assetManager_{}
    , playerTexture_(assetManager_.getTexture(TextureID::Player))
    , player_(playerTexture_)
    , tileData_(toIndex(TileID::Count)) {

    window_.setFramerateLimit(120);
    player_.setOrigin({24.f, 24.f});
    player_.setScale({scale_, scale_});
    player_.setSpeed(500.f);
    player_.physicsData_.type = Entity::PhysicsData::Dynamic;

    camera_.setSize({1600, 1080});
    camera_.setCenter(player_.getPosition());

    TileInfo tile;
    tile.texture = TextureID::Terrain;
    tile.rect.resize(9);
    tile.solid = false;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            tile.rect[x + y * 3] = {{x * 16, y * 16}, {16, 16}};
        }
    }
    tileData_[toIndex(TileID::Grass)] = tile;

    tileWidth_ = baseWidth_ * tileScale_;
    tileHeight_ = baseHeight_ * tileScale_;

    loadMapFromFile("level1.map");
    allEntities_.push_back(&player_);
}

void Game::run() {
    sf::Clock clock;

    while (window_.isOpen()) {
        HandleInput();

        float deltaTime = clock.restart().asSeconds();
        switch (gameMode_) {
            case GameMode::Play:
                updatePlay(deltaTime);
                break;
            case GameMode::LevelEditor:
                updateLevelEditor(deltaTime);
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

    sf::Vector2i offset = {tileWidth_ / 2, tileHeight_ / 2};
    sf::Sprite sprite(assetManager_.getTexture(TextureID::Terrain));
    sprite.setScale({tileScale_, tileScale_});
    sprite.setOrigin({baseWidth_ / 2.f, baseHeight_ / 2.f});
    for (const Tile &tile : tileStates_) {
        const TileInfo &info = tileData_[toIndex(tile.id)];
        sprite.setTextureRect(info.rect[tile.variant]);
        sf::Vector2f pos = static_cast<sf::Vector2f>(tile.position * tileWidth_ + offset);
        sprite.setPosition(pos);
        window_.draw(sprite);
    }

    switch (gameMode_) {
        case GameMode::Play:
            break;
        case GameMode::LevelEditor:
            drawLevelEditor();
            break;
    }

    // Render Game

    window_.draw(player_);

    window_.setView(window_.getDefaultView());

    // Render UI

    window_.display();
}

void Game::drawLevelEditor() {
    float x = std::floor(mousePosView_.x / tileWidth_);
    float y = std::floor(mousePosView_.y / tileHeight_);
    const TileInfo &info = tileData_[toIndex(TileID::Grass)];
    sf::Sprite sprite(assetManager_.getTexture(info.texture));
    sprite.setTextureRect(info.rect[tileOptionIdx_]);
    sprite.setScale({tileScale_, tileScale_});
    sprite.setOrigin({baseWidth_ / 2.f, baseHeight_ / 2.f});
    sprite.setPosition({
        x * tileWidth_ + tileWidth_ / 2, y * tileHeight_ + tileHeight_ / 2
    });
    window_.draw(sprite);
}

void Game::updatePlay(float deltaTime) {
    updatePhysics(deltaTime);
}

void Game::updateLevelEditor(float deltaTime) {
    camera_.move(playerVelocity_ * 900.f * deltaTime);
}

void Game::updatePhysics(float deltaTime) {
    for (Entity* &entity : allEntities_) {
        if (entity->physicsData_.type == Entity::PhysicsData::Dynamic) {
            entity->move(
                playerVelocity_ * entity->physicsData_.speed * deltaTime,
                camera_);
            // TODO check collisions
        }
    }
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
            if (key->scancode == sf::Keyboard::Scancode::T) {
                if (gameMode_ == GameMode::Play) {
                    gameMode_ = GameMode::LevelEditor;
                }
                else if (gameMode_ == GameMode::LevelEditor) {
                    camera_.setCenter(player_.getPosition());
                    gameMode_ = GameMode::Play;
                }
            }
            else if (key->scancode == sf::Keyboard::Scancode::O) {
                if (gameMode_ == GameMode::LevelEditor) {
                    saveMapToFile("level1.map");
                }
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        velocity += {0.f, -1.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        velocity += {0.f, 1.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity += {1.f, 0.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity += {-1.f, 0.f};
    }

    playerVelocity_ = math::normalize(velocity);
}

void Game::handleLevelEditorInput() {
    if (scrollWheelInput_ == ScrollWheelInput::Up) {
        tileOptionIdx_++;
        if (tileOptionIdx_ >= 9) {
            tileOptionIdx_ = 0;
        }
    }
    else if (scrollWheelInput_ == ScrollWheelInput::Down) {
        tileOptionIdx_--;
        if (tileOptionIdx_ < 0) {
            tileOptionIdx_ = 8;
        }
    }

    int x = std::floor(mousePosView_.x / tileWidth_);
    int y = std::floor(mousePosView_.y / tileHeight_);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        createTile(x, y);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        deleteTile(x, y);
    }

    sf::Vector2f velocity = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        velocity += {0.f, -1.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        velocity += {0.f, 1.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity += {1.f, 0.f};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity += {-1.f, 0.f};
    }

    playerVelocity_ = math::normalize(velocity);
}

void Game::createTile(int x, int y) {
    sf::Vector2i coords = {x, y};
    Tile tile{TileID::Grass, tileOptionIdx_, coords};
    for (size_t i = 0; i < tileStates_.size(); i++) {
        if (tileStates_[i].position == tile.position) {
            tileStates_[i] = tileStates_.back();
            tileStates_.pop_back();
            break;
        }
    }

    tileStates_.push_back(tile);
}

void Game::deleteTile(int x, int y) {
    sf::Vector2i tileToDeletePos = {x, y};
    for (size_t i = 0; i < tileStates_.size(); i++) {
        if (tileStates_[i].position == tileToDeletePos) {
            tileStates_[i] = tileStates_.back();
            tileStates_.pop_back();
            return;
        }
    }
}

void Game::saveMapToFile(const std::string &filename) {
    std::ofstream level{"../resources/levels/" + filename, std::ios::binary};
    if (!level.is_open()) {
        throw std::ios_base::failure("Couldn't open file\n");
    }

    size_t size = tileStates_.size();
    level.write(reinterpret_cast<const char *>(&size), sizeof(size));
    if (size > 0) {
        level.write(reinterpret_cast<const char *>(tileStates_.data()),
                    size * sizeof(Tile));
    }
}

void Game::loadMapFromFile(const std::string &filename) {
    std::ifstream level{"../resources/levels/" + filename, std::ios::binary};
    if (!level.is_open()) {
        throw std::ios_base::failure("Couldn't open file\n");
    }

    size_t size;
    level.read(reinterpret_cast<char *>(&size), sizeof(size));
    if (size > 0) {
        tileStates_.resize(size);
        level.read(reinterpret_cast<char *>(tileStates_.data()),
                   size * sizeof(Tile));
    }
}

template<typename T> requires std::is_enum_v<T>
constexpr size_t Game::toIndex(T id) {
    return static_cast<size_t>(id);
}
