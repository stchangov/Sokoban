// Copyright 2024 Stephan Tchangov
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace SB {
enum class Direction {
    Up, Down, Left, Right
};

enum class Tile {
    PlayerStart = '@',
    Empty = '.',
    Wall = '#',
    Box = 'A',
    Storage = 'a',
    BoxOnStorage = '1'
};

class Sokoban : public sf::Drawable {
 public:
    static const size_t TILE_SIZE = 64;

    Sokoban();
    explicit Sokoban(const std::string&);  // Optional

    unsigned int pixelHeight() const;  // Optional
    unsigned int pixelWidth() const;  // Optional

    unsigned int height() const { return _height; }
    unsigned int width() const { return _width; }

    sf::Vector2u playerLoc() const { return _playerLoc; }

    bool isWon() const;

    void movePlayer(Direction dir);
    void reset();

    friend std::ostream& operator<<(std::ostream& out, const Sokoban& s);
    friend std::istream& operator>>(std::istream& in, Sokoban& s);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    unsigned int _width;
    unsigned int _height;
    std::vector<std::vector<Tile>> _grid;
    sf::Vector2u _playerLoc;

    std::vector<sf::Vector2u> _boxLocations;
    std::vector<sf::Vector2u> _storageLocations;

    // Initial positions for reset function
    sf::Vector2u _initialPlayerLoc;
    std::vector<sf::Vector2u> _initialBoxLocations;
    std::vector<sf::Vector2u> _initialStorageLocations;

    // Structure textures
    sf::Texture _wall;
    sf::Texture _box;
    sf::Texture _empty;
    sf::Texture _storage;

    // Player textures
    sf::Texture _playerFacingUp;
    sf::Texture _playerFacingDown;
    sf::Texture _playerFacingLeft;
    sf::Texture _playerFacingRight;

    // Direction player will face
    Direction _playerDirection;
};

}  // namespace SB
