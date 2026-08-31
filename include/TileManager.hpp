#pragma once
#include "AssetManager.hpp"
#include "MathFunctions.hpp"

enum class TileID {
    Empty = -1,
    Grass,
    GrassGround,
    GrassAngles,
    GrassWater,
    Count,
};

struct Tile {
    TileID id = TileID::Empty;
    int option = 0;
};

class TileManager {

public:
    TileManager();

    void createTile(int x, int y);
    void deleteTile(int x, int y);
    void saveMapToFile(const std::string &filename);
    void loadMapFromFile(const std::string &filename);
    void changeTileOption(int delta);
    TextureID getTextureId(TileID tile) const;
    sf::IntRect getTextureRect(TileID id, int option) const;
    Tile getTileAt(int x, int y) const;
    int getGridWidth() const;
    int getGridHeight() const;
    float getTileScale() const;
    int getTileSize() const;
    int getBaseSize() const;

    int tileOption_ = 0;
    TileID tileGroupOption_ = TileID::Grass;

    struct TileGroup {
        TextureID texture = TextureID::Terrain;
        sf::Vector2i gridSize;
        sf::Vector2i offset;
        bool isSolid;
    };

    std::array<TileGroup, math::toIndex(TileID::Count)> tileGroups_;

private:

    float tileScale_ = 7.f;
    int baseSize_ = 16;
    int tileSize_ = baseSize_ * tileScale_;
    int gridWidth_ = 256;
    int gridHeight_ = 256;
    std::vector<Tile> tiles_;
};
