
# Sokoban Game

## Overview

This project is a Sokoban game built with C++ and SFML. Players solve puzzles by pushing boxes to specific spots in a grid. The game allows loading custom levels from files for more replayability.

## Features

- Uses SFML to provide a smooth graphical interface.
- Load custom levels from text files.
- Handles player movement, box pushing, and win detection.
- Reads and checks level files for smooth play.
- Optimized rendering with SFML textures and sprites.
- Built-in timer using SFML’s clock to track game time.

## Installation

### Prerequisites
- C++ Compiler (recommend C++17 or newer)
- SFML Library (2.5.1 or newer)

### Build Instructions
1. Clone this repository.
2. Make sure SFML is installed on your system.
3. Compile with:
   ```bash
   g++ main.cpp Sokoban.cpp -o Sokoban -lsfml-graphics -lsfml-window -lsfml-system
   ```
4. Run the game:
   ```bash
   ./Sokoban <level_file>
   ```

## How to Play
- Use the arrow keys to move the player and push boxes onto storage spots.
- You win when all boxes are correctly placed.
- Level data is read from text files.

## Controls
- Arrow keys: Move the player.

## Level File Format
- The game reads level files formatted as follows:
  - `@`: Player’s start position
  - `#`: Wall
  - `A`: Box
  - `a`: Storage location
  - `1`: Box on a storage location
  - `.`: Empty tile
