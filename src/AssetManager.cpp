#include "AssetManager.hpp"

AssetManager::AssetManager()
    : tileSets_{static_cast<size_t>(TextureID::Count)} {
    bool ok;
    ok = tileSets_[static_cast<size_t>(TextureID::Terrain)].loadFromFile("../resources/tiles/terrain.png");
    ok = tileSets_[static_cast<size_t>(TextureID::Player)].loadFromFile("../resources/Entities/cat.png");
    ok = tileSets_[static_cast<size_t>(TextureID::Rat)].loadFromFile("../resources/Entities/rat.png");
}

const sf::Texture& AssetManager::getTexture(TextureID id) const {
    return tileSets_[static_cast<size_t>(id)];
}
