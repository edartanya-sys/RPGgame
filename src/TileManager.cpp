#include <fstream>
#include "TileManager.hpp"

#include <iostream>

TileManager::TileManager()
    : tiles_(matrixWidth_ * matrixHeight_, Tile{}) {
    tileGroups_ = {
        {
            {TextureID::Terrain, {2, 2}, {0, 0}, false},
            {TextureID::Terrain, {3, 3}, {0, 0}, false},
            {TextureID::Terrain, {3, 3}, {0, 0}, false},
            {TextureID::Terrain, {4, 3}, {0, 0}, false},
        }
    };

    int cursorY = 0;
    for (auto &g : tileGroups_) {
        g.offset.y = cursorY;
        cursorY += g.gridSize.y * tileSize_;
    }
}

void TileManager::createTile(int x, int y) {
    if (x < 0 || y < 0) return;
    tiles_[y * matrixWidth_ + x ] = {tileGroupOption_, tileOption_};
}

void TileManager::deleteTile(int x, int y) {
    if (x < 0 || y < 0) return;
    tiles_[y * matrixWidth_ + x].id = TileID::Empty;
}

sf::IntRect TileManager::getTextureRect(TileID id, int option) const {
    const TileGroup &group = tileGroups_[math::toIndex(id)];
    int cols = group.gridSize.x;
    int localX = option % cols;
    int localY = option / cols;

    return {
        {
            group.offset.x + localX * tileSize_,
            group.offset.y + localY * tileSize_,
        },
        {
            tileSize_,
            tileSize_
        }
    };
}

Tile TileManager::getTileAt(int x, int y) const {
    return tiles_[y * matrixWidth_ + x];
}

int TileManager::getWidth() const {
    return matrixWidth_;
}

int TileManager::getHeight() const {
    return matrixHeight_;
}

void TileManager::saveMapToFile(const std::string &filename) {
    std::ofstream level{"../resources/levels/" + filename, std::ios::binary};
    if (!level.is_open()) {
        throw std::ios_base::failure("Couldn't open file\n");
    }

    size_t size = tiles_.size();
    level.write(reinterpret_cast<const char *>(&size), sizeof(size));
    if (size > 0) {
        level.write(reinterpret_cast<const char *>(tiles_.data()),
                    size * sizeof(Tile));
    }
}

void TileManager::loadMapFromFile(const std::string &filename) {
    std::ifstream level{"../resources/levels/" + filename, std::ios::binary};
    if (!level.is_open()) {
        throw std::ios_base::failure("Couldn't open file\n");
    }

    size_t size;
    level.read(reinterpret_cast<char *>(&size), sizeof(size));
    if (size > 0) {
        tiles_.resize(size);
        level.read(reinterpret_cast<char *>(tiles_.data()),
                   size * sizeof(Tile));
    }
}

void TileManager::changeTileOption(int delta) {
    if (delta == 0) {
        return;
    }

    tileOption_ += delta;
    sf::Vector2i grid = tileGroups_[math::toIndex(tileGroupOption_)].gridSize;
    int size = grid.x * grid.y;
    if (tileOption_ >= size) {
        tileOption_ = 0;
    }
    else if (tileOption_ < 0) {
        tileOption_ = size - 1;
    }
}

TextureID TileManager::getTextureId(TileID tileId) const {
    return tileGroups_[math::toIndex(tileId)].texture;
}
