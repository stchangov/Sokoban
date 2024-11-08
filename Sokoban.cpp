// Copyright 2024 Stephan Tchangov
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Sokoban.hpp"

namespace SB {

Sokoban::Sokoban() : _width(0), _height(0) {}

Sokoban::Sokoban(const std::string& filename)  {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file >> *this;

    if (!_wall.loadFromFile("block_06.png")) {
        throw std::runtime_error("Failed to load texture: block_06.png");
    }
    if (!_box.loadFromFile("crate_03.png")) {
        throw std::runtime_error("Failed to load texture: crate_03.png");
    }
    if (!_empty.loadFromFile("ground_01.png")) {
        throw std::runtime_error("Failed to load texture: ground_01.png");
    }
    if (!_storage.loadFromFile("ground_04.png")) {
        throw std::runtime_error("Failed to load texture: ground_04.png");
    }
    if (!_playerFacingUp.loadFromFile("player_08.png")) {  // Forward
        throw std::runtime_error("Failed to load texture: player_08.png");
    }
    if (!_playerFacingDown.loadFromFile("player_05.png")) {  // Down
        throw std::runtime_error("Failed to load texture: player_05.png");
    }
    if (!_playerFacingLeft.loadFromFile("player_20.png")) {  // Left
        throw std::runtime_error("Failed to load texture: player_20.png");
    }
    if (!_playerFacingRight.loadFromFile("player_17.png")) {  // Right
        throw std::runtime_error("Failed to load texture: player_17.png");
    }
}

bool Sokoban::isWon() const {
    size_t boxesInStorage = 0;
    for (const auto& boxLoc : _boxLocations) {
        // Compare box positions with storage positions
        bool found = std::find_if(
            _storageLocations.begin(),
             _storageLocations.end(),
            [boxLoc](const sf::Vector2u& storageLoc) {
                return storageLoc == boxLoc;
        }) != _storageLocations.end();  // If box is in a storage location

        if (found) {
            ++boxesInStorage;
        }
    }

    if (_boxLocations.size() < _storageLocations.size()) {
        // More storage locations than boxes: check if all boxes are in storage
        return boxesInStorage == _boxLocations.size();
    } else {
        // More boxes than storage locations or equal
        return boxesInStorage == _storageLocations.size();
    }
}

void Sokoban::movePlayer(Direction dir) {
    if (isWon()) {
        return;
    }

    sf::Vector2u newLoc(_playerLoc.x, _playerLoc.y);
    // Moving player
    switch (dir) {
        case Direction::Up:
            newLoc.y--;
            _playerDirection = Direction::Up;
            break;
        case Direction::Down:
            newLoc.y++;
            _playerDirection = Direction::Down;
            break;
        case Direction::Left:
            newLoc.x--;
            _playerDirection = Direction::Left;
            break;
        case Direction::Right:
            newLoc.x++;
            _playerDirection = Direction::Right;
            break;
    }

    // Check if the new position is within bounds
    if (newLoc.x >= _width || newLoc.y >= _height) {
        return;
    }

    // Check the tile at the new position
    Tile tileAtPlayerLoc = _grid[newLoc.y][newLoc.x];
    if (tileAtPlayerLoc == Tile::Wall) {
        return;
    }

    // If we find a box
    auto boxIt = std::find(_boxLocations.begin(), _boxLocations.end(), newLoc);
    if (boxIt != _boxLocations.end()) {
        size_t boxIndex = boxIt - _boxLocations.begin();
        sf::Vector2u newBoxLoc = *boxIt;  // Box loc for scanning nearby
        switch (dir) {
            case Direction::Up: newBoxLoc.y--; break;
            case Direction::Down: newBoxLoc.y++; break;
            case Direction::Left: newBoxLoc.x--; break;
            case Direction::Right: newBoxLoc.x++; break;
            default: break;
        }

        if (newBoxLoc.x >= _width || newBoxLoc.y >= _height) {
            return;
        }

        Tile tileAtNewBoxLoc =
             _grid[newBoxLoc.y][newBoxLoc.x];
        auto foundBoxIt = std::find(
            _boxLocations.begin(),
             _boxLocations.end(),
              newBoxLoc);
        if (tileAtNewBoxLoc == Tile::Wall ||
            foundBoxIt != _boxLocations.end()) {
            return;
        }

        _boxLocations[boxIndex] = newBoxLoc;
    }
    _playerLoc = newLoc;
}

void Sokoban::reset() {
    _playerLoc = _initialPlayerLoc;
    _boxLocations = _initialBoxLocations;
    _playerDirection = Direction::Down;
}

std::ostream &operator<<(std::ostream& out, const Sokoban& s) {
    for (size_t row = 0; row < s._height; ++row) {
        for (size_t col = 0; col < s._width; ++col) {
            switch (s._grid[row][col]) {
                case Tile::PlayerStart: out << '@'; break;
                case Tile::Empty: out << '.'; break;
                case Tile::Wall: out << '#'; break;
                case Tile::Box: out << 'A'; break;
                case Tile::Storage: out << '1'; break;
                case Tile::BoxOnStorage: out << 'a'; break;
                default: out << '?';
            }
        }
        out << '\n';
    }
    return out;
}

std::istream &operator>>(std::istream& in, Sokoban& s) {
    in >> s._height >> s._width;
    s._grid.resize(s._height, std::vector<Tile>(s._width, Tile::Empty));

    for (size_t row = 0; row < s._height; ++row) {
        for (size_t col = 0; col < s._width; ++col) {
            char tileChar;
            in >> tileChar;
            switch (tileChar) {
                case '@':
                    s._grid[row][col] = Tile::PlayerStart;
                    s._playerLoc = sf::Vector2u(col, row);
                    s._initialPlayerLoc = sf::Vector2u(col, row);  // For reset
                    break;
                case '.':
                    s._grid[row][col] = Tile::Empty;
                    break;
                case '#':
                    s._grid[row][col] = Tile::Wall;
                    break;
                case 'A':
                    s._grid[row][col] = Tile::Box;
                    s._boxLocations.push_back(sf::Vector2u(col, row));
                    s._initialBoxLocations.push_back(sf::Vector2u(col, row));
                    break;
                case 'a':
                    s._grid[row][col] = Tile::Storage;
                    s._storageLocations.push_back(sf::Vector2u(col, row));
                    break;
                case '1':
                    s._grid[row][col] = Tile::BoxOnStorage;
                    break;
                default:
                    throw std::invalid_argument("Invalid character.");
                    break;
            }
        }
        in.ignore();
    }
    return in;
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite playerSprite;

    sf::Sprite tileSprite;
    for (size_t row = 0; row < _height; ++row) {
        for (size_t col = 0; col < _width; ++col) {
            tileSprite.setTexture(_empty);  // Set to floor texture
            tileSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            target.draw(tileSprite);
        }
    }

    for (size_t row = 0; row < _height; ++row) {
        for (size_t col = 0; col < _width; ++col) {
            sf::Sprite currentSprite;
            switch (_grid[row][col]) {  // Switch on a single tile
                case Tile::Wall:
                    currentSprite.setTexture(_wall);
                    break;
                case Tile::Empty:
                    currentSprite.setTexture(_empty);
                    break;
                case Tile::Storage:
                    currentSprite.setTexture(_storage);
                    break;
                case Tile::PlayerStart:
                    playerSprite.setTexture(_playerFacingDown);
                    break;
                default:
                    continue;
            }

            currentSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            target.draw(currentSprite);
            }
        }

        // Draw all boxes
        sf::Sprite boxSprite;
        boxSprite.setTexture(_box);
        for (const auto& boxPos : _boxLocations) {
            boxSprite.setPosition(boxPos.x * TILE_SIZE, boxPos.y * TILE_SIZE);
            target.draw(boxSprite);
        }

        // Load texture for which direction player is facing
        switch (_playerDirection) {
            case Direction::Up:
                playerSprite.setTexture(_playerFacingUp);
                break;
            case Direction::Down:
                playerSprite.setTexture(_playerFacingDown);
                break;
            case Direction::Left:
                playerSprite.setTexture(_playerFacingLeft);
                break;
            case Direction::Right:
                playerSprite.setTexture(_playerFacingRight);
                break;
            default: break;
        }
        playerSprite.setPosition(
            _playerLoc.x * TILE_SIZE,
            _playerLoc.y * TILE_SIZE);

        target.draw(playerSprite);

        // Victory Message
        if (isWon()) {
            sf::Font font;
            if (!font.loadFromFile("PixelifySans-VariableFont_wght.ttf")) {
                std::__throw_runtime_error
                ("Failed to load font for victory text.");
            }

            sf::Text victoryText;
            victoryText.setString("Victory!");
            victoryText.setFont(font);
            victoryText.setCharacterSize(100);
            victoryText.setFillColor(sf::Color::White);
            victoryText.setOrigin(victoryText.getGlobalBounds().width / 2,
                                  victoryText.getGlobalBounds().height / 2);

            victoryText.setPosition((_width * TILE_SIZE) / 2,
                                    (_height * TILE_SIZE) / 2 - 50);

            target.draw(victoryText);
        }
}

}  // namespace SB
