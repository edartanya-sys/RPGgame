#pragma once
#include "AssetManager.hpp"
#include "TileManager.hpp"
#include "World.hpp"

class Renderer {
public:
    Renderer(AssetManager &assets, float tileScale, int baseSize);

    void drawTiles(sf::RenderWindow &window, const TileManager &tileManager);
    void drawBounds(sf::RenderWindow &window, const TileManager &tileManager);
    void drawLevelEditor(sf::RenderWindow &window,
                         const TileManager &tileManager,
                         sf::Vector2f mousePosView);
    void drawEntities(sf::RenderWindow &window, World &world);

private:
    AssetManager &assets_;
    float tileScale_ = 0.f;
    int baseSize_ = 0;
    int tileSize_ = baseSize_ * tileScale_;
};
