#pragma once
#include <SFML/Graphics.hpp>

enum class TextureID {
    Terrain,
    Player,
    Count
};

class AssetManager {

public:
    AssetManager();

    const sf::Texture& getTexture(TextureID id);

private:
    std::vector<sf::Texture> tileSets_{static_cast<size_t>(TextureID::Count)};
};

