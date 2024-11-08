// Copyright 2024 Stephan Tchangov
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

#include "Sokoban.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

using SB::Sokoban;
using SB::Direction;

BOOST_AUTO_TEST_CASE(testMovement) {
    Sokoban game("level1.lvl");

    game.movePlayer(Direction::Down);
    sf::Vector2u updatedLoc = game.playerLoc();
    sf::Vector2u expectedLoc = { 3, 7 };  // One tile down

    BOOST_REQUIRE_EQUAL(updatedLoc.x, expectedLoc.x);
    BOOST_REQUIRE_EQUAL(updatedLoc.y, expectedLoc.y);
}

BOOST_AUTO_TEST_CASE(testMoveOffScreen) {
    Sokoban game("level1.lvl");

    game.movePlayer(Direction::Down);
    game.movePlayer(Direction::Down);
    game.movePlayer(Direction::Down);
    game.movePlayer(Direction::Down);
    game.movePlayer(Direction::Down);
    game.movePlayer(Direction::Down);
    sf::Vector2u playerLoc = game.playerLoc();

    BOOST_REQUIRE(playerLoc.x >= 0 && playerLoc.x < game.width());
    BOOST_REQUIRE(playerLoc.y >= 0 && playerLoc.y < game.height());
}

BOOST_AUTO_TEST_CASE(testBounds) {
    Sokoban game("level1.lvl");

    game.movePlayer(Direction::Left);  // 2, 6
    game.movePlayer(Direction::Left);  // 1, 6
    game.movePlayer(Direction::Left);  // Should not move due to wall

    sf::Vector2u playerLoc = game.playerLoc();

    BOOST_REQUIRE_EQUAL(playerLoc.x, 1);
    BOOST_REQUIRE_EQUAL(playerLoc.y, 6);
}

BOOST_AUTO_TEST_CASE(testBoundsNoWalls) {
    Sokoban game("level4.lvl");

    game.movePlayer(Direction::Up);
    game.movePlayer(Direction::Up);

    sf::Vector2u playerLoc = game.playerLoc();
    sf::Vector2u expectedLoc = { 6, 0 };

    BOOST_REQUIRE_EQUAL(playerLoc.x, expectedLoc.x);
    BOOST_REQUIRE_EQUAL(playerLoc.y, expectedLoc.y);
}

BOOST_AUTO_TEST_CASE(testWindowDimensions) {
    Sokoban game("level3.lvl");

    unsigned int height = 12;
    unsigned int width = 10;

    BOOST_REQUIRE_EQUAL(game.height(), height);
    BOOST_REQUIRE_EQUAL(game.width(), width);
}

BOOST_AUTO_TEST_CASE(testMovementAgainst2Crates) {
    Sokoban game("level2.lvl");

    game.movePlayer(Direction::Up);  // should stay at 8, 5
    sf::Vector2u playerLoc = game.playerLoc();
    sf::Vector2u expectedLoc = { 8, 5 };

    BOOST_REQUIRE_EQUAL(playerLoc.x, expectedLoc.x);
    BOOST_REQUIRE_EQUAL(playerLoc.y, expectedLoc.y);
}
