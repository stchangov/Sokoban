// Copyright 2024 Stephan Tchangov
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Sokoban.hpp"

void updateGameTime(sf::Clock& clock, sf::Text& timerText, bool isPaused) {
    if (!isPaused) {  // If not paused keep updating time
        int totalSeconds = static_cast<int>(clock.getElapsedTime().asSeconds());

        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        std::string secondsStr = (seconds < 10 ? "0" : "") +
                                std::to_string(seconds);

        timerText.setString("Time: " + std::to_string(minutes) + ":" +
                            secondsStr);
    }
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Usage: ./Sokoban <level_file>");
    }

    SB::Sokoban game(argv[1]);

    sf::Font font;
    if (!font.loadFromFile("Roboto-BlackItalic.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }

    sf::Clock clock;
    bool isPaused = false;

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 10);

    unsigned int windowWidth = (game.width() * SB::Sokoban::TILE_SIZE);
    unsigned int windowHeight = (game.height() * SB::Sokoban::TILE_SIZE);
    sf::RenderWindow window(sf::VideoMode(windowWidth,
                                        windowHeight), "Sokoban");

    sf::Music music;
    if (!music.openFromFile("_8_Bit_Nostalgia_.ogg")) {
        throw::std::runtime_error("Failed to load music.");
    }

    bool victoryMusicPlayed = false;

    while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::Left:
                            game.movePlayer(SB::Direction::Left);
                            break;
                        case sf::Keyboard::Right:
                            game.movePlayer(SB::Direction::Right);
                            break;
                        case sf::Keyboard::Up:
                            game.movePlayer(SB::Direction::Up);
                            break;
                        case sf::Keyboard::Down:
                            game.movePlayer(SB::Direction::Down);
                            break;
                        case sf::Keyboard::R:
                            game.reset();
                            clock.restart();
                            isPaused = false;
                            music.stop();
                            victoryMusicPlayed = false;
                        default: break;
                    }
                }
            }

            if (game.isWon() && !victoryMusicPlayed) {
                music.play();
                victoryMusicPlayed = true;
                isPaused = true;
            }

            updateGameTime(clock, timerText, isPaused);

            window.clear();
            window.draw(game);
            window.draw(timerText);
            window.display();
        }

    std::cout << game << std::endl;

    return 0;
}
