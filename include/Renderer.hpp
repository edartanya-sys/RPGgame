#pragma once
#include "AssetManager.hpp"
#include "TileManager.hpp"
#include "World.hpp"

class Renderer {
public:
    Renderer(AssetManager &assets);

    void drawTiles(sf::RenderWindow &window, const TileManager &tileManager);
    void drawBounds(sf::RenderWindow &window, const TileManager &tileManager);
    void drawLevelEditor(sf::RenderWindow &window,
                         const TileManager &tileManager,
                         sf::Vector2f mousePosView);
    void drawEntities(sf::RenderWindow &window, World &world);

private:
    AssetManager &assets_;
    float scale_ = 5.f;
    float tileScale_ = 7.f;
    int baseWidth_ = 16;
    int baseHeight_ = 16;
    int tileWidth_ = baseWidth_ * tileScale_;
    int tileHeight_ = baseHeight_ * tileScale_;
};
