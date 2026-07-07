#include "AssetManager.hpp"

AssetManager::AssetManager()
    : tileSets_{static_cast<size_t>(TextureID::Count)} {
    bool ok;
    ok = tileSets_[static_cast<size_t>(TextureID::Terrain)].loadFromFile("../resources/tiles/tileSet.png");
    ok = tileSets_[static_cast<size_t>(TextureID::Player)].loadFromFile("../resources/Entities/cat.png");
}

const sf::Texture& AssetManager::getTexture(TextureID id) {
    return tileSets_[static_cast<size_t>(id)];
}
