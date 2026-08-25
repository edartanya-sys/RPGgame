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
    int getWidth() const;
    int getHeight() const;

    int tileOption_ = 0;
    TileID tileGroupOption_ = TileID::Grass;

    struct TileGroup {
        TextureID texture = TextureID::Terrain;
        sf::Vector2i gridSize;
        sf::Vector2i offset;
        bool isSolid;
    };

    int tileSize_ = 16;
    int matrixWidth_ = 512;
    int matrixHeight_ = 512;
    std::array<TileGroup, math::toIndex(TileID::Count)> tileGroups_;
    std::vector<Tile> tiles_;
};
