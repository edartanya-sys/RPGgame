#include "Renderer.hpp"
#include <cassert>

Renderer::Renderer(AssetManager &assets, float tileScale, int baseSize)
    : assets_{assets}
    , tileScale_{tileScale}
    , baseSize_{baseSize} {
    tileSize_ = baseSize_ * tileScale_;
    assert(tileScale_ > 0 && baseSize_ > 0);
}

void Renderer::drawTiles(sf::RenderWindow &window,
                         const TileManager &tileManager) {
    sf::Sprite sprite{assets_.getTexture(TextureID::Terrain)};
    sprite.setScale({tileScale_, tileScale_});

    for (int y = 0; y < tileManager.getGridHeight(); ++y) {
        for (int x = 0; x < tileManager.getGridWidth(); ++x) {
            const Tile &tile = tileManager.getTileAt(x, y);
            if (tile.id == TileID::Empty) {
                continue;
            }
            float coordX = x * tileSize_;
            float coordY = y * tileSize_;
            sprite.setPosition({coordX, coordY});
            sprite.setTexture(
                assets_.getTexture(tileManager.getTextureId(tile.id)));
            sprite.setTextureRect(
                tileManager.getTextureRect(tile.id, tile.option));
            window.draw(sprite);
        }
    }
}

void Renderer::drawBounds(sf::RenderWindow &window,
                          const TileManager &tileManager) {
    sf::RectangleShape bounds{
        {
            static_cast<float>(tileManager.getGridWidth() * tileSize_),
            static_cast<float>(tileManager.getGridHeight() * tileSize_)
        }
    };
    bounds.setPosition({0.f, 0.f});
    bounds.setFillColor({40, 40, 40, 100});
    bounds.setOutlineColor({255, 80, 80});
    bounds.setOutlineThickness(3.f);
    window.draw(bounds);
}

void Renderer::drawLevelEditor(sf::RenderWindow &window,
                               const TileManager &tileManager,
                               sf::Vector2f mousePosView) {
    float x = std::floor(mousePosView.x / tileSize_);
    float y = std::floor(mousePosView.y / tileSize_);
    Tile tile;
    tile.id = tileManager.tileGroupOption_;
    tile.option = tileManager.tileOption_;
    sf::Sprite sprite(
        assets_.getTexture(
            tileManager.tileGroups_[math::toIndex(tile.id)].texture));
    sprite.setTextureRect(tileManager.getTextureRect(tile.id, tile.option));

    sprite.setScale({tileScale_, tileScale_});
    sprite.setOrigin({baseSize_ / 2.f, baseSize_ / 2.f});
    sprite.setPosition({
        x * tileSize_ + tileSize_ / 2, y * tileSize_ + tileSize_ / 2
    });
    window.draw(sprite);
}

void Renderer::drawEntities(sf::RenderWindow &window, World &world) {
    auto &sprites = world.storage<SpriteComponent>();
    auto &positions = world.storage<Position>();
    sf::Font a{"../resources/fonts/times.ttf"};
    sf::Text healthText{a};
    healthText.setCharacterSize(80);
    healthText.setFillColor(sf::Color::Red);

    for (std::uint32_t i = 0; i < sprites.getSize(); ++i) {
        Entity entity = sprites.getEntityAt(i);
        if (world.storage<Player>().has(entity)) {
            int health = world.storage<HealthComponent>().get(entity).value;
            healthText.setString(std::to_string(health));
            healthText.setPosition(world.storage<Position>().get(entity).value - sf::Vector2f{800.f, 540.f});
        }
        sf::RectangleShape box;
        if (world.storage<Collider>().has(entity)) {
            auto &collider = world.storage<Collider>().get(entity).rect;
            box.setSize({collider.size.x, collider.size.y});
            box.setPosition({positions.get(entity).value.x + collider.position.x,
                             positions.get(entity).value.y + collider.position.y});
            box.setFillColor({255, 0, 0, 80});
        }
        SpriteComponent &sprite = sprites.get(entity);
        sprite.sprite.setPosition(positions.get(entity).value);
        window.draw(sprite.sprite);
        window.draw(healthText);
        window.draw(box);
    }
}
