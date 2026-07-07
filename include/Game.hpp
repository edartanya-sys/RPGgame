#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "AssetManager.hpp"

class Game {

public:
    Game();

    enum class GameMode {
        Play,
        LevelEditor
    };

    enum class ScrollWheelInput {
        Up,
        Down,
        None
    };

    enum class TileID {
        Grass,
        Count,
    };

    void run();

private:
    void updatePlay(float deltaTime);
    void updateLevelEditor(float deltaTime);
    void updatePhysics(float deltaTime);
    void HandleInput();
    void handlePlayInput();
    void handleLevelEditorInput();
    void draw();

    void createTile(int x, int y);
    void deleteTile(int x, int y);
    void drawLevelEditor();

    void saveMapToFile(const std::string &filename);
    void loadMapFromFile(const std::string &filename);

    template<typename T> requires std::is_enum_v<T>
    constexpr size_t toIndex(T id);

    GameMode gameMode_ = GameMode::Play;
    ScrollWheelInput scrollWheelInput_ = ScrollWheelInput::None;

    sf::RenderWindow window_;
    sf::View camera_{};

    AssetManager assetManager_{};
    sf::Texture playerTexture_{};

    sf::Vector2i mousePosWindow_;
    sf::Vector2f mousePosView_;

    Entity player_;
    std::vector<Entity*> allEntities_;

    sf::Vector2f playerVelocity_;

    struct Tile {
        TileID id;
        int variant = 0;
        sf::Vector2i position = {0, 0};

        Tile() = default;
        Tile(TileID id, int variant, sf::Vector2i position)
            : id(id)
            , variant(variant)
            , position(position) {}
    };

    struct TileInfo {
        TextureID texture;
        std::vector<sf::IntRect> rect;
        bool solid;
    };

    int tileOptionIdx_ = 0;
    std::vector<TileInfo> tileData_{};
    std::vector<Tile> tileStates_{};
    float scale_ = 5.f;
    float tileScale_ = 7.f;
    int baseWidth_ = 16;
    int baseHeight_ = 16;
    int tileWidth_ = baseWidth_ * tileScale_;
    int tileHeight_ = baseHeight_ * tileScale_;
};

